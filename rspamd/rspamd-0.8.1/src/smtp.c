/*
 * Copyright (c) 2009-2012, Vsevolod Stakhov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "main.h"
#include "cfg_file.h"
#include "cfg_xml.h"
#include "util.h"
#include "smtp.h"
#include "smtp_proto.h"
#include "smtp_utils.h"
#include "map.h"
#include "message.h"
#include "settings.h"
#include "dns.h"
#include "lua/lua_common.h"

/* Max line size as it is defined in rfc2822 */
#define OUTBUFSIZ 1000

/* Upstream timeouts */
#define DEFAULT_UPSTREAM_ERROR_TIME 10
#define DEFAULT_UPSTREAM_DEAD_TIME 300
#define DEFAULT_UPSTREAM_MAXERRORS 10

#define DEFAULT_REJECT_MESSAGE "450 4.5.0 Spam message rejected"

static gboolean smtp_write_socket (void *arg);

/* Init functions */
gpointer init_smtp (struct rspamd_config *cfg);
void start_smtp (struct rspamd_worker *worker);

worker_t smtp_worker = {
	"smtp",                     /* Name */
	init_smtp,                  /* Init function */
	start_smtp,                 /* Start function */
	TRUE,                       /* Has socket */
	FALSE,                      /* Non unique */
	FALSE,                      /* Non threaded */
	TRUE,                       /* Killable */
	SOCK_STREAM                 /* TCP socket */
};

static gboolean
call_stage_filters (struct smtp_session *session, enum rspamd_smtp_stage stage)
{
	gboolean res = TRUE;
	GList *list = session->ctx->smtp_filters[stage];
	struct smtp_filter *filter;

	while (list) {
		filter = list->data;
		if (!filter->filter (session, filter->filter_data)) {
			res = FALSE;
			break;
		}
		list = g_list_next (list);
	}

	return res;
}

static gboolean
read_smtp_command (struct smtp_session *session, rspamd_fstring_t *line)
{
	struct smtp_command *cmd;
	gchar outbuf[BUFSIZ];
	gint r;

	if (!parse_smtp_command (session, line, &cmd)) {
		session->error = SMTP_ERROR_BAD_COMMAND;
		session->errors++;
		return FALSE;
	}

	switch (cmd->command) {
	case SMTP_COMMAND_HELO:
	case SMTP_COMMAND_EHLO:
		if (session->state == SMTP_STATE_GREETING || session->state ==
			SMTP_STATE_HELO) {
			if (parse_smtp_helo (session, cmd)) {
				session->state = SMTP_STATE_FROM;
			}
			else {
				session->errors++;
			}
			if (!call_stage_filters (session, SMTP_STAGE_HELO)) {
				return FALSE;
			}
			return TRUE;
		}
		else {
			goto improper_sequence;
		}
		break;
	case SMTP_COMMAND_QUIT:
		session->state = SMTP_STATE_QUIT;
		break;
	case SMTP_COMMAND_NOOP:
		break;
	case SMTP_COMMAND_MAIL:
		if (((session->state == SMTP_STATE_GREETING || session->state ==
			SMTP_STATE_HELO) && !session->ctx->helo_required)
			|| session->state == SMTP_STATE_FROM) {
			if (parse_smtp_from (session, cmd)) {
				session->state = SMTP_STATE_RCPT;
			}
			else {
				session->errors++;
				return FALSE;
			}
			if (!call_stage_filters (session, SMTP_STAGE_MAIL)) {
				return FALSE;
			}
		}
		else {
			goto improper_sequence;
		}
		break;
	case SMTP_COMMAND_RCPT:
		if (session->state == SMTP_STATE_RCPT) {
			if (parse_smtp_rcpt (session, cmd)) {
				if (!call_stage_filters (session, SMTP_STAGE_RCPT)) {
					return FALSE;
				}
				/* Make upstream connection */
				if (session->upstream == NULL) {
					if (!create_smtp_upstream_connection (session)) {
						session->error = SMTP_ERROR_UPSTREAM;
						session->state = SMTP_STATE_CRITICAL_ERROR;
						return FALSE;
					}
				}
				else {
					/* Send next rcpt to upstream */
					session->state = SMTP_STATE_WAIT_UPSTREAM;
					session->upstream_state = SMTP_STATE_BEFORE_DATA;
					rspamd_dispatcher_restore (session->upstream_dispatcher);
					r = rspamd_snprintf (outbuf, sizeof (outbuf), "RCPT TO: ");
					r += smtp_upstream_write_list (session->rcpt->data,
							outbuf + r,
							sizeof (outbuf) - r);
					session->cur_rcpt = NULL;
					return rspamd_dispatcher_write (
						session->upstream_dispatcher,
						outbuf,
						r,
						FALSE,
						FALSE);
				}
				session->state = SMTP_STATE_WAIT_UPSTREAM;
				return TRUE;
			}
			else {
				session->errors++;
				return FALSE;
			}
		}
		else {
			goto improper_sequence;
		}
		break;
	case SMTP_COMMAND_RSET:
		session->from = NULL;
		if (session->rcpt) {
			g_list_free (session->rcpt);
		}
		if (session->upstream) {
			remove_normal_event (session->s,
				smtp_upstream_finalize_connection,
				session);
			session->upstream = NULL;
		}
		session->state = SMTP_STATE_GREETING;
		break;
	case SMTP_COMMAND_DATA:
		if (session->state == SMTP_STATE_RCPT) {
			if (session->rcpt == NULL) {
				session->error = SMTP_ERROR_RECIPIENTS;
				session->errors++;
				return FALSE;
			}
			if (!call_stage_filters (session, SMTP_STAGE_DATA)) {
				return FALSE;
			}
			if (session->upstream == NULL) {
				session->error = SMTP_ERROR_UPSTREAM;
				session->state = SMTP_STATE_CRITICAL_ERROR;
				return FALSE;
			}
			else {
				session->upstream_state = SMTP_STATE_DATA;
				rspamd_dispatcher_restore (session->upstream_dispatcher);
				r = rspamd_snprintf (outbuf, sizeof (outbuf), "DATA" CRLF);
				session->state = SMTP_STATE_WAIT_UPSTREAM;
				session->error = SMTP_ERROR_DATA_OK;
				return rspamd_dispatcher_write (session->upstream_dispatcher,
						   outbuf,
						   r,
						   FALSE,
						   FALSE);
			}
		}
		else {
			goto improper_sequence;
		}
	case SMTP_COMMAND_VRFY:
	case SMTP_COMMAND_EXPN:
	case SMTP_COMMAND_HELP:
		session->error = SMTP_ERROR_UNIMPLIMENTED;
		return FALSE;
	}

	session->error = SMTP_ERROR_OK;
	return TRUE;

improper_sequence:
	session->errors++;
	session->error = SMTP_ERROR_SEQUENCE;
	return FALSE;
}

static gboolean
process_smtp_data (struct smtp_session *session)
{
	struct stat st;
	gint r;
	GList *cur, *t;
	rspamd_fstring_t *f;
	gchar *s;

	if (fstat (session->temp_fd, &st) == -1) {
		msg_err ("fstat failed: %s", strerror (errno));
		goto err;
	}
	/* Now mmap temp file if it is small enough */
	session->temp_size = st.st_size;
	if (session->ctx->max_size == 0 || st.st_size <
		(off_t)session->ctx->max_size) {
		session->task = rspamd_task_new (session->worker);
		session->task->resolver = session->resolver;
		session->task->fin_callback = smtp_write_socket;
		session->task->fin_arg = session;
		session->task->msg =
			rspamd_mempool_alloc (session->pool, sizeof (GString));
		session->task->s = session->s;
#ifdef HAVE_MMAP_NOCORE
		if ((session->task->msg->str =
			mmap (NULL, st.st_size, PROT_READ, MAP_SHARED | MAP_NOCORE,
			session->temp_fd, 0)) == MAP_FAILED) {
#else
		if ((session->task->msg->str =
			mmap (NULL, st.st_size, PROT_READ, MAP_SHARED, session->temp_fd,
			0)) == MAP_FAILED) {
#endif
			msg_err ("mmap failed: %s", strerror (errno));
			goto err;
		}
		session->task->msg->len = st.st_size;
		session->task->helo = session->helo;
		/* Save MAIL FROM */
		cur = session->from;
		if (cur) {
			f = cur->data;
			s = rspamd_mempool_alloc (session->pool, f->len + 1);
			rspamd_strlcpy (s, f->begin, f->len + 1);
			session->task->from = s;
		}
		/* Save recipients */
		t = session->rcpt;
		while (t) {
			cur = t->data;
			if (cur) {
				f = cur->data;
				s = rspamd_mempool_alloc (session->pool, f->len + 1);
				rspamd_strlcpy (s, f->begin, f->len + 1);
				session->task->rcpt = g_list_prepend (session->task->rcpt, s);
			}
			t = g_list_next (t);
		}

		memcpy (&session->task->from_addr, &session->client_addr,
			sizeof (struct in_addr));
		session->task->cmd = CMD_CHECK;

		if (process_message (session->task) == -1) {
			msg_err ("cannot process message");
			munmap (session->task->msg->str, st.st_size);
			goto err;
		}
		if (session->task->cfg->pre_filters == NULL) {
			r = rspamd_process_filters (session->task);
			if (r == -1) {
				msg_err ("cannot process message");
				munmap (session->task->msg->str, st.st_size);
				goto err;
			}
		}
		else {
			rspamd_lua_call_pre_filters (session->task);
			/* We want fin_task after pre filters are processed */
			session->task->s->wanna_die = TRUE;
			session->task->state = WAIT_PRE_FILTER;
			check_session_pending (session->task->s);
		}
	}
	else {
		msg_info ("not scan message as it is %z bytes and maximum is %z",
			st.st_size,
			session->ctx->max_size);
		session->task = NULL;
		return smtp_send_upstream_message (session);
	}

	return TRUE;
err:
	session->error = SMTP_ERROR_FILE;
	session->state = SMTP_STATE_CRITICAL_ERROR;
	if (!rspamd_dispatcher_write (session->dispatcher, session->error, 0, FALSE,
		TRUE)) {
		return FALSE;
	}
	destroy_session (session->s);
	return FALSE;
}

/*
 * Callback that is called when there is data to read in buffer
 */
static gboolean
smtp_read_socket (rspamd_fstring_t * in, void *arg)
{
	struct smtp_session *session = arg;

	switch (session->state) {
	case SMTP_STATE_RESOLVE_REVERSE:
	case SMTP_STATE_RESOLVE_NORMAL:
	case SMTP_STATE_DELAY:
		session->error = make_smtp_error (session->pool,
				550,
				"%s Improper use of SMTP command pipelining",
				"5.5.0");
		session->state = SMTP_STATE_ERROR;
		break;
	case SMTP_STATE_GREETING:
	case SMTP_STATE_HELO:
	case SMTP_STATE_FROM:
	case SMTP_STATE_RCPT:
	case SMTP_STATE_DATA:
		read_smtp_command (session, in);
		if (session->state != SMTP_STATE_WAIT_UPSTREAM) {
			if (session->errors > session->ctx->max_errors) {
				session->error = SMTP_ERROR_LIMIT;
				session->state = SMTP_STATE_CRITICAL_ERROR;
				if (!rspamd_dispatcher_write (session->dispatcher,
					session->error, 0, FALSE, TRUE)) {
					return FALSE;
				}
				destroy_session (session->s);
				return FALSE;
			}
			if (!smtp_write_socket (session)) {
				return FALSE;
			}
		}
		break;
	case SMTP_STATE_AFTER_DATA:
		if (in->len == 0) {
			return TRUE;
		}
		if (in->len == 3 &&
			memcmp (in->begin, DATA_END_TRAILER, in->len) == 0) {
			return process_smtp_data (session);
		}

		if (write (session->temp_fd, in->begin, in->len) != (ssize_t)in->len) {
			msg_err ("cannot write to temp file: %s", strerror (errno));
			session->error = SMTP_ERROR_FILE;
			session->state = SMTP_STATE_CRITICAL_ERROR;
			if (!rspamd_dispatcher_write (session->dispatcher, session->error,
				0, FALSE, TRUE)) {
				return FALSE;
			}
			destroy_session (session->s);
			return FALSE;
		}
		break;
	case SMTP_STATE_WAIT_UPSTREAM:
		rspamd_dispatcher_pause (session->dispatcher);
		break;
	default:
		session->error = make_smtp_error (session->pool,
				550,
				"%s Internal error",
				"5.5.0");
		session->state = SMTP_STATE_ERROR;
		break;
	}

	if (session->state == SMTP_STATE_QUIT) {
		destroy_session (session->s);
		return FALSE;
	}
	else if (session->state == SMTP_STATE_WAIT_UPSTREAM) {
		rspamd_dispatcher_pause (session->dispatcher);
	}

	return TRUE;
}

/*
 * Callback for socket writing
 */
static gboolean
smtp_write_socket (void *arg)
{
	struct smtp_session *session = arg;

	if (session->state == SMTP_STATE_CRITICAL_ERROR) {
		if (session->error != NULL) {
			if (!rspamd_dispatcher_write (session->dispatcher, session->error,
				0, FALSE, TRUE)) {
				return FALSE;
			}
		}
		destroy_session (session->s);
		return FALSE;
	}
	else if (session->state == SMTP_STATE_END) {
		if (session->task != NULL) {
			return write_smtp_reply (session);
		}
		else {
			if (session->error != NULL) {
				if (!rspamd_dispatcher_write (session->dispatcher,
					session->error, 0, FALSE, TRUE)) {
					return FALSE;
				}
			}
		}
	}
	else {
		if (session->error != NULL) {
			if (!rspamd_dispatcher_write (session->dispatcher, session->error,
				0, FALSE, TRUE)) {
				return FALSE;
			}
		}
	}

	return TRUE;
}

/*
 * Called if something goes wrong
 */
static void
smtp_err_socket (GError * err, void *arg)
{
	struct smtp_session *session = arg;

	msg_info ("abnormally closing connection, error: %s", err->message);
	/* Free buffers */
	destroy_session (session->s);
}

/*
 * Write greeting to client
 */
static gboolean
write_smtp_greeting (struct smtp_session *session)
{
	if (session->ctx->smtp_banner) {
		if (!rspamd_dispatcher_write (session->dispatcher,
			session->ctx->smtp_banner, 0, FALSE, TRUE)) {
			return FALSE;
		}
	}

	return TRUE;
}

/*
 * Return from a delay
 */
static void
smtp_delay_handler (gint fd, short what, void *arg)
{
	struct smtp_session *session = arg;

	remove_normal_event (session->s,
		(event_finalizer_t)event_del,
		session->delay_timer);
	if (session->state == SMTP_STATE_DELAY) {
		session->state = SMTP_STATE_GREETING;
		write_smtp_greeting (session);
	}
	else {
		session->state = SMTP_STATE_CRITICAL_ERROR;
		(void)smtp_write_socket (session);
	}
}

/*
 * Make delay for a client
 */
static void
smtp_make_delay (struct smtp_session *session)
{
	struct event *tev;
	struct timeval *tv;
	gint32 jitter;

	if (session->ctx->smtp_delay != 0 && session->state == SMTP_STATE_DELAY) {
		tev = rspamd_mempool_alloc (session->pool, sizeof (struct event));
		tv = rspamd_mempool_alloc (session->pool, sizeof (struct timeval));
		if (session->ctx->delay_jitter != 0) {
			jitter = g_random_int_range (0, session->ctx->delay_jitter);
			msec_to_tv (session->ctx->smtp_delay + jitter, tv);
		}
		else {
			msec_to_tv (session->ctx->smtp_delay, tv);
		}

		evtimer_set (tev, smtp_delay_handler, session);
		evtimer_add (tev, tv);
		register_async_event (session->s,
			(event_finalizer_t)event_del,
			tev,
			g_quark_from_static_string ("smtp proxy"));
		session->delay_timer = tev;
	}
	else if (session->state == SMTP_STATE_DELAY) {
		session->state = SMTP_STATE_GREETING;
		write_smtp_greeting (session);
	}
}

/*
 * Handle DNS replies
 */
static void
smtp_dns_cb (struct rspamd_dns_reply *reply, void *arg)
{
	struct smtp_session *session = arg;
	gint res = 0;
	union rspamd_reply_element *elt;
	GList *cur;

	switch (session->state) {
	case SMTP_STATE_RESOLVE_REVERSE:
		/* Parse reverse reply and start resolve of this ip */
		if (reply->code != RDNS_RC_NOERROR) {
			rspamd_conditional_debug (rspamd_main->logger,
				session->client_addr.s_addr,
				__FUNCTION__,
				"DNS error: %s",
				dns_strerror (reply->code));

			if (reply->code == RDNS_RC_NXDOMAIN) {
				session->hostname = rspamd_mempool_strdup (session->pool,
						XCLIENT_HOST_UNAVAILABLE);
			}
			else {
				session->hostname = rspamd_mempool_strdup (session->pool,
						XCLIENT_HOST_TEMPFAIL);
			}
			session->state = SMTP_STATE_DELAY;
			smtp_make_delay (session);
		}
		else {
			if (reply->elements) {
				elt = reply->elements->data;
				session->hostname = rspamd_mempool_strdup (session->pool,
						elt->ptr.name);
				session->state = SMTP_STATE_RESOLVE_NORMAL;
				make_dns_request (session->resolver, session->s, session->pool,
					smtp_dns_cb, session, RDNS_REQUEST_A, session->hostname);

			}
		}
		break;
	case SMTP_STATE_RESOLVE_NORMAL:
		if (reply->code != RDNS_RC_NOERROR) {
			rspamd_conditional_debug (rspamd_main->logger,
				session->client_addr.s_addr,
				__FUNCTION__,
				"DNS error: %s",
				dns_strerror (reply->code));

			if (reply->code == RDNS_RC_NXDOMAIN) {
				session->hostname = rspamd_mempool_strdup (session->pool,
						XCLIENT_HOST_UNAVAILABLE);
			}
			else {
				session->hostname = rspamd_mempool_strdup (session->pool,
						XCLIENT_HOST_TEMPFAIL);
			}
			session->state = SMTP_STATE_DELAY;
			smtp_make_delay (session);
		}
		else {
			res = 0;
			cur = reply->elements;
			while (cur) {
				elt = cur->data;
				if (memcmp (&session->client_addr, &elt->a.addr[0],
					sizeof (struct in_addr)) == 0) {
					res = 1;
					session->resolved = TRUE;
					break;
				}
				cur = g_list_next (cur);
			}

			if (res == 0) {
				msg_info ("cannot find address for hostname: %s, ip: %s",
					session->hostname,
					inet_ntoa (session->client_addr));
				session->hostname = rspamd_mempool_strdup (session->pool,
						XCLIENT_HOST_UNAVAILABLE);
			}
			session->state = SMTP_STATE_DELAY;
			smtp_make_delay (session);
		}
		break;
	case SMTP_STATE_ERROR:
		session->state = SMTP_STATE_WRITE_ERROR;
		smtp_write_socket (session);
		break;
	default:
		/*
		 * This callback is called on unknown state, usually this indicates
		 * an error (invalid pipelining)
		 */
		break;
	}
}

/*
 * Accept new connection and construct task
 */
static void
accept_socket (gint fd, short what, void *arg)
{
	struct rspamd_worker *worker = (struct rspamd_worker *)arg;
	union sa_union su;
	struct smtp_session *session;
	struct smtp_worker_ctx *ctx;

	socklen_t addrlen = sizeof (su.ss);
	gint nfd;

	if ((nfd =
		accept_from_socket (fd, (struct sockaddr *)&su.ss, &addrlen)) == -1) {
		msg_warn ("accept failed: %s", strerror (errno));
		return;
	}
	/* Check for EAGAIN */
	if (nfd == 0) {
		return;
	}

	ctx = worker->ctx;
	session = g_malloc0 (sizeof (struct smtp_session));
	session->pool = rspamd_mempool_new (rspamd_mempool_suggest_size ());

	if (su.ss.ss_family == AF_UNIX) {
		msg_info ("accepted connection from unix socket");
		session->client_addr.s_addr = INADDR_NONE;
	}
	else if (su.ss.ss_family == AF_INET) {
		msg_info ("accepted connection from %s port %d",
			inet_ntoa (su.s4.sin_addr), ntohs (su.s4.sin_port));
		memcpy (&session->client_addr, &su.s4.sin_addr,
			sizeof (struct in_addr));
	}

	session->sock = nfd;
	session->temp_fd = -1;
	session->worker = worker;
	session->ctx = ctx;
	session->cfg = worker->srv->cfg;
	session->session_time = time (NULL);
	session->resolver = ctx->resolver;
	session->ev_base = ctx->ev_base;
	worker->srv->stat->connections_count++;

	/* Resolve client's addr */
	/* Set up async session */
	session->s = new_async_session (session->pool,
			NULL,
			NULL,
			free_smtp_session,
			session);
	session->state = SMTP_STATE_RESOLVE_REVERSE;
	if (!make_dns_request (session->resolver, session->s, session->pool,
		smtp_dns_cb, session, RDNS_REQUEST_PTR, &session->client_addr)) {
		msg_err ("cannot resolve %s", inet_ntoa (session->client_addr));
		g_free (session);
		close (nfd);
		return;
	}
	else {
		session->dispatcher = rspamd_create_dispatcher (session->ev_base,
				nfd,
				BUFFER_LINE,
				smtp_read_socket,
				smtp_write_socket,
				smtp_err_socket,
				&session->ctx->smtp_timeout,
				session);
		session->dispatcher->peer_addr = session->client_addr.s_addr;
	}
}

static void
parse_smtp_banner (struct smtp_worker_ctx *ctx, const gchar *line)
{
	gint hostmax, banner_len = sizeof ("220 ") - 1;
	gchar *p, *t, *hostbuf = NULL;
	gboolean has_crlf = FALSE;

	p = (gchar *)line;
	while (*p) {
		if (*p == '%') {
			p++;
			switch (*p) {
			case 'n':
				/* Assume %n as CRLF */
				banner_len += sizeof (CRLF) - 1 + sizeof ("220 -") - 1;
				has_crlf = TRUE;
				break;
			case 'h':
				hostmax = sysconf (_SC_HOST_NAME_MAX) + 1;
				hostbuf = alloca (hostmax);
				gethostname (hostbuf, hostmax);
				hostbuf[hostmax - 1] = '\0';
				banner_len += strlen (hostbuf);
				break;
			case '%':
				banner_len += 1;
				break;
			default:
				banner_len += 2;
				break;
			}
		}
		else {
			banner_len++;
		}
		p++;
	}

	if (has_crlf) {
		banner_len += sizeof (CRLF "220 " CRLF);
	}
	else {
		banner_len += sizeof (CRLF);
	}

	ctx->smtp_banner = rspamd_mempool_alloc (ctx->pool, banner_len + 1);
	t = ctx->smtp_banner;
	p = (gchar *)line;

	if (has_crlf) {
		t = g_stpcpy (t, "220-");
	}
	else {
		t = g_stpcpy (t, "220 ");
	}

	while (*p) {
		if (*p == '%') {
			p++;
			switch (*p) {
			case 'n':
				/* Assume %n as CRLF */
				*t++ = CR; *t++ = LF;
				t = g_stpcpy (t, "220-");
				p++;
				break;
			case 'h':
				t = g_stpcpy (t, hostbuf);
				p++;
				break;
			case '%':
				*t++ = '%';
				p++;
				break;
			default:
				/* Copy all %<gchar> to dest */
				*t++ = *(p - 1); *t++ = *p;
				break;
			}
		}
		else {
			*t++ = *p++;
		}
	}
	if (has_crlf) {
		t = g_stpcpy (t, CRLF "220 " CRLF);
	}
	else {
		t = g_stpcpy (t, CRLF);
	}
}

static void
make_capabilities (struct smtp_worker_ctx *ctx, const gchar *line)
{
	gchar **strv, *p, *result, *hostbuf;
	guint32 num, i, len, hostmax;

	strv = g_strsplit_set (line, ",;", -1);
	num = g_strv_length (strv);

	hostmax = sysconf (_SC_HOST_NAME_MAX) + 1;
	hostbuf = alloca (hostmax);
	gethostname (hostbuf, hostmax);
	hostbuf[hostmax - 1] = '\0';

	len = sizeof ("250-") + strlen (hostbuf) + sizeof (CRLF) - 1;

	for (i = 0; i < num; i++) {
		p = strv[i];
		len += sizeof ("250-") + sizeof (CRLF) + strlen (p) - 2;
	}

	result = rspamd_mempool_alloc (ctx->pool, len);
	ctx->smtp_capabilities = result;

	p = result;
	if (num == 0) {
		p += rspamd_snprintf (p, len - (p - result), "250 %s" CRLF, hostbuf);
	}
	else {
		p += rspamd_snprintf (p, len - (p - result), "250-%s" CRLF, hostbuf);
		for (i = 0; i < num; i++) {
			if (i != num - 1) {
				p += rspamd_snprintf (p,
						len - (p - result),
						"250-%s" CRLF,
						strv[i]);
			}
			else {
				p += rspamd_snprintf (p,
						len - (p - result),
						"250 %s" CRLF,
						strv[i]);
			}
		}
	}

	g_strfreev (strv);
}

gpointer
init_smtp (struct rspamd_config *cfg)
{
	struct smtp_worker_ctx *ctx;
	GQuark type;

	type = g_quark_try_string ("smtp");

	ctx = g_malloc0 (sizeof (struct smtp_worker_ctx));
	ctx->pool = rspamd_mempool_new (rspamd_mempool_suggest_size ());

	/* Set default values */
	ctx->smtp_timeout_raw = 300000;
	ctx->smtp_delay = 0;
	ctx->smtp_banner = "220 ESMTP Ready." CRLF;
	bzero (ctx->smtp_filters, sizeof (GList *) * SMTP_STAGE_MAX);
	ctx->max_errors = DEFAULT_MAX_ERRORS;
	ctx->reject_message = DEFAULT_REJECT_MESSAGE;

	rspamd_rcl_register_worker_option (cfg, type, "upstreams",
		rspamd_rcl_parse_struct_string, ctx,
		G_STRUCT_OFFSET (struct smtp_worker_ctx, upstreams_str), 0);

	rspamd_rcl_register_worker_option (cfg, type, "banner",
		rspamd_rcl_parse_struct_string, ctx,
		G_STRUCT_OFFSET (struct smtp_worker_ctx, smtp_banner_str), 0);

	rspamd_rcl_register_worker_option (cfg, type, "timeout",
		rspamd_rcl_parse_struct_time, ctx,
		G_STRUCT_OFFSET (struct smtp_worker_ctx,
		smtp_timeout_raw), RSPAMD_CL_FLAG_TIME_UINT_32);

	rspamd_rcl_register_worker_option (cfg, type, "delay",
		rspamd_rcl_parse_struct_time, ctx,
		G_STRUCT_OFFSET (struct smtp_worker_ctx,
		smtp_delay), RSPAMD_CL_FLAG_TIME_UINT_32);

	rspamd_rcl_register_worker_option (cfg, type, "jitter",
		rspamd_rcl_parse_struct_time, ctx,
		G_STRUCT_OFFSET (struct smtp_worker_ctx,
		delay_jitter), RSPAMD_CL_FLAG_TIME_UINT_32);

	rspamd_rcl_register_worker_option (cfg, type, "capabilities",
		rspamd_rcl_parse_struct_string, ctx,
		G_STRUCT_OFFSET (struct smtp_worker_ctx, smtp_capabilities_str), 0);

	rspamd_rcl_register_worker_option (cfg, type, "xclient",
		rspamd_rcl_parse_struct_boolean, ctx,
		G_STRUCT_OFFSET (struct smtp_worker_ctx, use_xclient), 0);

	rspamd_rcl_register_worker_option (cfg, type, "reject_message",
		rspamd_rcl_parse_struct_string, ctx,
		G_STRUCT_OFFSET (struct smtp_worker_ctx, reject_message), 0);

	rspamd_rcl_register_worker_option (cfg, type, "max_errors",
		rspamd_rcl_parse_struct_integer, ctx,
		G_STRUCT_OFFSET (struct smtp_worker_ctx,
		max_errors), RSPAMD_CL_FLAG_INT_32);

	rspamd_rcl_register_worker_option (cfg, type, "max_size",
		rspamd_rcl_parse_struct_integer, ctx,
		G_STRUCT_OFFSET (struct smtp_worker_ctx,
		max_size), RSPAMD_CL_FLAG_INT_SIZE);

	return ctx;
}

/* Make post-init configuration */
static gboolean
config_smtp_worker (struct rspamd_worker *worker)
{
	struct smtp_worker_ctx *ctx = worker->ctx;
	gchar *value;

	/* Init timeval */
	msec_to_tv (ctx->smtp_timeout_raw, &ctx->smtp_timeout);

	/* Init upstreams */
	if ((value = ctx->upstreams_str) != NULL) {
		if (!parse_upstreams_line (ctx->pool, ctx->upstreams, value,
			&ctx->upstream_num)) {
			return FALSE;
		}
	}
	else {
		msg_err ("no upstreams defined, don't know what to do");
		return FALSE;
	}
	/* Create smtp banner */
	if ((value = ctx->smtp_banner_str) != NULL) {
		parse_smtp_banner (ctx, value);
	}

	/* Parse capabilities */
	if ((value = ctx->smtp_capabilities_str) != NULL) {
		make_capabilities (ctx, value);
	}

	return TRUE;
}


/*
 * Start worker process
 */
void
start_smtp (struct rspamd_worker *worker)
{
	struct smtp_worker_ctx *ctx = worker->ctx;

	ctx->ev_base = rspamd_prepare_worker (worker, "smtp_worker", accept_socket);

	/* Set smtp options */
	if ( !config_smtp_worker (worker)) {
		msg_err ("cannot configure smtp worker, exiting");
		exit (EXIT_SUCCESS);
	}

	/* Maps events */
	rspamd_map_watch (worker->srv->cfg, ctx->ev_base);

	/* DNS resolver */
	ctx->resolver = dns_resolver_init (ctx->ev_base, worker->srv->cfg);

	/* Set umask */
	umask (S_IWGRP | S_IWOTH | S_IROTH | S_IRGRP);

	event_base_loop (ctx->ev_base, 0);

	rspamd_log_close (rspamd_main->logger);
	exit (EXIT_SUCCESS);
}

void
register_smtp_filter (struct smtp_worker_ctx *ctx,
	enum rspamd_smtp_stage stage,
	smtp_filter_t filter,
	gpointer filter_data)
{
	struct smtp_filter *new;

	new = rspamd_mempool_alloc (ctx->pool, sizeof (struct smtp_filter));

	new->filter = filter;
	new->filter_data = filter_data;

	if (stage >= SMTP_STAGE_MAX) {
		msg_err ("invalid smtp stage: %d", stage);
	}
	else {
		ctx->smtp_filters[stage] =
			g_list_prepend (ctx->smtp_filters[stage], new);
	}
}

/*
 * vi:ts=4
 */
