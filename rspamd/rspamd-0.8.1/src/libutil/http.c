/* Copyright (c) 2014, Vsevolod Stakhov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *       * Redistributions of source code must retain the above copyright
 *         notice, this list of conditions and the following disclaimer.
 *       * Redistributions in binary form must reproduce the above copyright
 *         notice, this list of conditions and the following disclaimer in the
 *         documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
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
#include "http.h"
#include "utlist.h"
#include "util.h"
#include "printf.h"
#include "logger.h"
#include "ref.h"

#include <limits.h>

struct rspamd_http_connection_private {
	struct _rspamd_http_privbuf {
		GString *data;
		ref_entry_t ref;
	} *buf;
	gboolean new_header;
	struct rspamd_http_header *header;
	struct http_parser parser;
	struct http_parser_settings parser_cb;
	struct event ev;
	struct timeval tv;
	struct timeval *ptv;
	struct rspamd_http_message *msg;
	struct iovec *out;
	guint outlen;
	gsize wr_pos;
	gsize wr_total;
};

enum http_magic_type {
	HTTP_MAGIC_PLAIN = 0,
	HTTP_MAGIC_HTML,
	HTTP_MAGIC_CSS,
	HTTP_MAGIC_JS,
	HTTP_MAGIC_PNG,
	HTTP_MAGIC_JPG
};

static const struct _rspamd_http_magic {
	const gchar *ext;
	const gchar *ct;
} http_file_types[] = {
	[HTTP_MAGIC_PLAIN] = { "txt", "text/plain" },
	[HTTP_MAGIC_HTML] = { "html", "text/html" },
	[HTTP_MAGIC_CSS] = { "css", "text/css" },
	[HTTP_MAGIC_JS] = { "js", "application/javascript" },
	[HTTP_MAGIC_PNG] = { "png", "image/png" },
	[HTTP_MAGIC_JPG] = { "jpg", "image/jpeg" },
};

static gchar *http_week[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static gchar *http_month[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
							   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };


#define HTTP_ERROR http_error_quark ()
GQuark
http_error_quark (void)
{
	return g_quark_from_static_string ("http-error-quark");
}

static void
rspamd_http_privbuf_dtor (gpointer ud)
{
	struct _rspamd_http_privbuf *p = (struct _rspamd_http_privbuf *)ud;

	if (p->data) {
		g_string_free (p->data, TRUE);
	}
	g_slice_free1 (sizeof (struct _rspamd_http_privbuf), p);
}

static const gchar *
rspamd_http_code_to_str (gint code)
{
	if (code == 200) {
		return "OK";
	}
	else if (code == 404) {
		return "Not found";
	}
	else if (code == 403 || code == 401) {
		return "Not authorized";
	}
	else if (code >= 400 && code < 500) {
		return "Bad request";
	}
	else if (code >= 300 && code < 400) {
		return "See Other";
	}
	else if (code >= 500 && code < 600) {
		return "Internal server error";
	}

	return "Unknown error";
}

/*
 * Obtained from nginx
 * Copyright (C) Igor Sysoev
 */
static guint mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

time_t
rspamd_http_parse_date (const gchar *header, gsize len)
{
	const gchar *p, *end;
	gint month;
	guint day, year, hour, min, sec;
	guint64 time;
	enum {
		no = 0, rfc822, /* Tue, 10 Nov 2002 23:50:13   */
		rfc850, /* Tuesday, 10-Dec-02 23:50:13 */
		isoc /* Tue Dec 10 23:50:13 2002    */
	} fmt;

	fmt = 0;
	if (len > 0) {
		end = header + len;
	}
	else {
		end = header + strlen (header);
	}

#if (NGX_SUPPRESS_WARN)
	day = 32;
	year = 2038;
#endif

	for (p = header; p < end; p++) {
		if (*p == ',') {
			break;
		}

		if (*p == ' ') {
			fmt = isoc;
			break;
		}
	}

	for (p++; p < end; p++)
		if (*p != ' ') {
			break;
		}

	if (end - p < 18) {
		return (time_t)-1;
	}

	if (fmt != isoc) {
		if (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
			return (time_t)-1;
		}

		day = (*p - '0') * 10 + *(p + 1) - '0';
		p += 2;

		if (*p == ' ') {
			if (end - p < 18) {
				return (time_t)-1;
			}
			fmt = rfc822;

		}
		else if (*p == '-') {
			fmt = rfc850;

		}
		else {
			return (time_t)-1;
		}

		p++;
	}

	switch (*p) {

	case 'J':
		month = *(p + 1) == 'a' ? 0 : *(p + 2) == 'n' ? 5 : 6;
		break;

	case 'F':
		month = 1;
		break;

	case 'M':
		month = *(p + 2) == 'r' ? 2 : 4;
		break;

	case 'A':
		month = *(p + 1) == 'p' ? 3 : 7;
		break;

	case 'S':
		month = 8;
		break;

	case 'O':
		month = 9;
		break;

	case 'N':
		month = 10;
		break;

	case 'D':
		month = 11;
		break;

	default:
		return (time_t)-1;
	}

	p += 3;

	if ((fmt == rfc822 && *p != ' ') || (fmt == rfc850 && *p != '-')) {
		return (time_t)-1;
	}

	p++;

	if (fmt == rfc822) {
		if (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
			|| *(p + 2) < '0' || *(p + 2) > '9' || *(p + 3) < '0'
			|| *(p + 3) > '9') {
			return (time_t)-1;
		}

		year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
			+ (*(p + 2) - '0') * 10 + *(p + 3) - '0';
		p += 4;

	}
	else if (fmt == rfc850) {
		if (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
			return (time_t)-1;
		}

		year = (*p - '0') * 10 + *(p + 1) - '0';
		year += (year < 70) ? 2000 : 1900;
		p += 2;
	}

	if (fmt == isoc) {
		if (*p == ' ') {
			p++;
		}

		if (*p < '0' || *p > '9') {
			return (time_t)-1;
		}

		day = *p++ - '0';

		if (*p != ' ') {
			if (*p < '0' || *p > '9') {
				return (time_t)-1;
			}

			day = day * 10 + *p++ - '0';
		}

		if (end - p < 14) {
			return (time_t)-1;
		}
	}

	if (*p++ != ' ') {
		return (time_t)-1;
	}

	if (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
		return (time_t)-1;
	}

	hour = (*p - '0') * 10 + *(p + 1) - '0';
	p += 2;

	if (*p++ != ':') {
		return (time_t)-1;
	}

	if (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
		return (time_t)-1;
	}

	min = (*p - '0') * 10 + *(p + 1) - '0';
	p += 2;

	if (*p++ != ':') {
		return (time_t)-1;
	}

	if (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9') {
		return (time_t)-1;
	}

	sec = (*p - '0') * 10 + *(p + 1) - '0';

	if (fmt == isoc) {
		p += 2;

		if (*p++ != ' ') {
			return (time_t)-1;
		}

		if (*p < '0' || *p > '9' || *(p + 1) < '0' || *(p + 1) > '9'
			|| *(p + 2) < '0' || *(p + 2) > '9' || *(p + 3) < '0'
			|| *(p + 3) > '9') {
			return (time_t)-1;
		}

		year = (*p - '0') * 1000 + (*(p + 1) - '0') * 100
			+ (*(p + 2) - '0') * 10 + *(p + 3) - '0';
	}

	if (hour > 23 || min > 59 || sec > 59) {
		return (time_t)-1;
	}

	if (day == 29 && month == 1) {
		if ((year & 3) || ((year % 100 == 0) && (year % 400) != 0)) {
			return (time_t)-1;
		}

	}
	else if (day > mday[month]) {
		return (time_t)-1;
	}

	/*
	 * shift new year to March 1 and start months from 1 (not 0),
	 * it is needed for Gauss' formula
	 */

	if (--month <= 0) {
		month += 12;
		year -= 1;
	}

	/* Gauss' formula for Gregorian days since March 1, 1 BC */

	time = (guint64) (
	    /* days in years including leap years since March 1, 1 BC */

		365 * year + year / 4 - year / 100 + year / 400

	    /* days before the month */

		+ 367 * month / 12 - 30

	    /* days before the day */

		+ day - 1

	    /*
	     * 719527 days were between March 1, 1 BC and March 1, 1970,
	     * 31 and 28 days were in January and February 1970
	     */

		- 719527 + 31 + 28) * 86400 + hour * 3600 + min * 60 + sec;

	return (time_t) time;
}

static inline void
rspamd_http_check_date (struct rspamd_http_connection_private *priv)
{
	if (g_ascii_strcasecmp (priv->header->name->str, "date") == 0) {
		priv->msg->date = rspamd_http_parse_date (priv->header->value->str,
				priv->header->value->len);
	}
}

static gint
rspamd_http_on_url (http_parser * parser, const gchar *at, size_t length)
{
	struct rspamd_http_connection *conn =
		(struct rspamd_http_connection *)parser->data;
	struct rspamd_http_connection_private *priv;

	priv = conn->priv;

	g_string_append_len (priv->msg->url, at, length);

	return 0;
}

static gint
rspamd_http_on_status (http_parser * parser, const gchar *at, size_t length)
{
	struct rspamd_http_connection *conn =
		(struct rspamd_http_connection *)parser->data;
	struct rspamd_http_connection_private *priv;

	priv = conn->priv;

	if (parser->status_code != 200) {
		if (priv->msg->status == NULL) {
			priv->msg->status = g_string_sized_new (128);
		}
		g_string_append_len (priv->msg->status, at, length);
	}

	return 0;
}

static gint
rspamd_http_on_header_field (http_parser * parser,
	const gchar *at,
	size_t length)
{
	struct rspamd_http_connection *conn =
		(struct rspamd_http_connection *)parser->data;
	struct rspamd_http_connection_private *priv;

	priv = conn->priv;

	if (priv->header == NULL) {
		priv->header = g_slice_alloc (sizeof (struct rspamd_http_header));
		priv->header->name = g_string_sized_new (32);
		priv->header->value = g_string_sized_new (32);
	}
	else if (priv->new_header) {
		DL_APPEND (priv->msg->headers, priv->header);
		rspamd_http_check_date (priv);
		priv->header = g_slice_alloc (sizeof (struct rspamd_http_header));
		priv->header->name = g_string_sized_new (32);
		priv->header->value = g_string_sized_new (32);
	}

	priv->new_header = FALSE;
	g_string_append_len (priv->header->name, at, length);

	return 0;
}

static gint
rspamd_http_on_header_value (http_parser * parser,
	const gchar *at,
	size_t length)
{
	struct rspamd_http_connection *conn =
		(struct rspamd_http_connection *)parser->data;
	struct rspamd_http_connection_private *priv;

	priv = conn->priv;

	if (priv->header == NULL) {
		/* Should not happen */
		return -1;
	}

	priv->new_header = TRUE;
	g_string_append_len (priv->header->value, at, length);

	return 0;
}

static int
rspamd_http_on_headers_complete (http_parser * parser)
{
	struct rspamd_http_connection *conn =
		(struct rspamd_http_connection *)parser->data;
	struct rspamd_http_connection_private *priv;

	priv = conn->priv;

	if (priv->header != NULL) {
		DL_APPEND (priv->msg->headers, priv->header);
		rspamd_http_check_date (priv);
		priv->header = NULL;
	}

	if (parser->content_length != 0 && parser->content_length != ULLONG_MAX) {
		priv->msg->body = g_string_sized_new (parser->content_length + 1);
	}
	else {
		priv->msg->body = g_string_sized_new (BUFSIZ);
	}

	priv->msg->method = parser->method;
	priv->msg->code = parser->status_code;

	return 0;
}

static int
rspamd_http_on_body (http_parser * parser, const gchar *at, size_t length)
{
	struct rspamd_http_connection *conn =
		(struct rspamd_http_connection *)parser->data;
	struct rspamd_http_connection_private *priv;

	priv = conn->priv;

	g_string_append_len (priv->msg->body, at, length);

	if (conn->opts & RSPAMD_HTTP_BODY_PARTIAL) {
		return (conn->body_handler (conn, priv->msg, at, length));
	}

	return 0;
}

static int
rspamd_http_on_message_complete (http_parser * parser)
{
	struct rspamd_http_connection *conn =
		(struct rspamd_http_connection *)parser->data;
	struct rspamd_http_connection_private *priv;
	int ret = 0;

	priv = conn->priv;

	if (conn->body_handler != NULL) {
		rspamd_http_connection_ref (conn);
		if ((conn->opts & RSPAMD_HTTP_BODY_PARTIAL) == 0) {
			ret = conn->body_handler (conn,
					priv->msg,
					priv->msg->body->str,
					priv->msg->body->len);
		}
		rspamd_http_connection_unref (conn);
	}

	if (ret == 0) {
		rspamd_http_connection_ref (conn);
		ret = conn->finish_handler (conn, priv->msg);
		conn->finished = TRUE;
		rspamd_http_connection_unref (conn);

		/* Disable reading if we have read an HTTP message */
		if (event_pending (&priv->ev, EV_READ, NULL)) {
			event_del (&priv->ev);
		}
	}

	return ret;
}

static void
rspamd_http_simple_client_helper (struct rspamd_http_connection *conn)
{
	struct event_base *base;

	base = conn->priv->ev.ev_base;
	rspamd_http_connection_reset (conn);
	/* Plan read message */
	rspamd_http_connection_read_message (conn, conn->ud, conn->fd,
			conn->priv->ptv, base);
}

static void
rspamd_http_write_helper (struct rspamd_http_connection *conn)
{
	struct rspamd_http_connection_private *priv;
	struct iovec *start;
	guint niov, i;
	gint flags = 0;
	gsize remain;
	gssize r;
	GError *err;
	struct iovec *cur_iov;
	struct msghdr msg;

	priv = conn->priv;

	if (priv->wr_pos == priv->wr_total) {
		goto call_finish_handler;
	}

	start = &priv->out[0];
	niov = priv->outlen;
	remain = priv->wr_pos;
	/* We know that niov is small enough for that */
	cur_iov = alloca (niov * sizeof (struct iovec));
	memcpy (cur_iov, priv->out, niov * sizeof (struct iovec));
	for (i = 0; i < priv->outlen && remain > 0; i++) {
		/* Find out the first iov required */
		start = &cur_iov[i];
		if (start->iov_len <= remain) {
			remain -= start->iov_len;
			start = &cur_iov[i + 1];
			niov--;
		}
		else {
			start->iov_base = (void *)((char *)start->iov_base + remain);
			start->iov_len -= remain;
			remain = 0;
		}
	}

	memset (&msg, 0, sizeof (msg));
	msg.msg_iov = start;
	msg.msg_iovlen = MIN (IOV_MAX, niov);
#ifdef MSG_NOSIGNAL
	flags = MSG_NOSIGNAL;
#endif
	r = sendmsg (conn->fd, &msg, flags);

	if (r == -1) {
		err =
			g_error_new (HTTP_ERROR, errno, "IO write error: %s", strerror (
					errno));
		rspamd_http_connection_ref (conn);
		conn->error_handler (conn, err);
		rspamd_http_connection_unref (conn);
		g_error_free (err);
		return;
	}
	else {
		priv->wr_pos += r;
	}

	if (priv->wr_pos >= priv->wr_total) {
		goto call_finish_handler;
	}
	else {
		/* Want to write more */
		event_add (&priv->ev, priv->ptv);
	}

	return;

call_finish_handler:
	if ((conn->opts & RSPAMD_HTTP_CLIENT_SIMPLE) == 0) {
		rspamd_http_connection_ref (conn);
		conn->finished = TRUE;
		conn->finish_handler (conn, priv->msg);
		rspamd_http_connection_unref (conn);
	}
	else {
		/* Plan read message */
		rspamd_http_simple_client_helper (conn);
	}
}

static void
rspamd_http_event_handler (int fd, short what, gpointer ud)
{
	struct rspamd_http_connection *conn = (struct rspamd_http_connection *)ud;
	struct rspamd_http_connection_private *priv;
	struct _rspamd_http_privbuf *pbuf;
	GString *buf;
	gssize r;
	GError *err;

	priv = conn->priv;
	pbuf = priv->buf;
	REF_RETAIN (pbuf);
	rspamd_http_connection_ref (conn);
	buf = priv->buf->data;

	if (what == EV_READ) {
		r = read (fd, buf->str, buf->allocated_len);
		if (r == -1) {
			err = g_error_new (HTTP_ERROR,
					errno,
					"IO read error: %s",
					strerror (errno));
			conn->error_handler (conn, err);
			g_error_free (err);

			REF_RELEASE (pbuf);
			rspamd_http_connection_unref (conn);

			return;
		}
		else if (r == 0) {
			if (conn->finished) {
				REF_RELEASE (pbuf);
				rspamd_http_connection_unref (conn);
				/* Double unref to avoid dangling connections */
				rspamd_http_connection_unref (conn);
				return;
			}
			else {
				err = g_error_new (HTTP_ERROR,
						errno,
						"IO read error: unexpected EOF");
				conn->error_handler (conn, err);
				g_error_free (err);

				REF_RELEASE (pbuf);
				rspamd_http_connection_unref (conn);

				return;
			}
		}
		else {
			buf->len = r;

			if (http_parser_execute (&priv->parser, &priv->parser_cb, buf->str,
				r) != (size_t)r) {
				err = g_error_new (HTTP_ERROR, priv->parser.http_errno,
						"HTTP parser error: %s",
						http_errno_description (priv->parser.http_errno));
				conn->error_handler (conn, err);
				g_error_free (err);

				REF_RELEASE (pbuf);
				rspamd_http_connection_unref (conn);

				return;
			}
		}
	}
	else if (what == EV_TIMEOUT) {
		err = g_error_new (HTTP_ERROR, ETIMEDOUT,
				"IO timeout");
		conn->error_handler (conn, err);
		g_error_free (err);

		REF_RELEASE (pbuf);
		rspamd_http_connection_unref (conn);

		return;
	}
	else if (what == EV_WRITE) {
		rspamd_http_write_helper (conn);
	}

	REF_RELEASE (pbuf);
	rspamd_http_connection_unref (conn);
}

static void
rspamd_http_parser_reset (struct rspamd_http_connection *conn)
{
	struct rspamd_http_connection_private *priv = conn->priv;

	http_parser_init (&priv->parser,
		conn->type == RSPAMD_HTTP_SERVER ? HTTP_REQUEST : HTTP_RESPONSE);

	priv->parser_cb.on_url = rspamd_http_on_url;
	priv->parser_cb.on_status = rspamd_http_on_status;
	priv->parser_cb.on_header_field = rspamd_http_on_header_field;
	priv->parser_cb.on_header_value = rspamd_http_on_header_value;
	priv->parser_cb.on_headers_complete = rspamd_http_on_headers_complete;
	priv->parser_cb.on_body = rspamd_http_on_body;
	priv->parser_cb.on_message_complete = rspamd_http_on_message_complete;
}

struct rspamd_http_connection *
rspamd_http_connection_new (rspamd_http_body_handler_t body_handler,
	rspamd_http_error_handler_t error_handler,
	rspamd_http_finish_handler_t finish_handler,
	unsigned opts,
	enum rspamd_http_connection_type type)
{
	struct rspamd_http_connection *new;
	struct rspamd_http_connection_private *priv;

	if (error_handler == NULL || finish_handler == NULL) {
		return NULL;
	}

	new = g_slice_alloc0 (sizeof (struct rspamd_http_connection));
	new->opts = opts;
	new->type = type;
	new->body_handler = body_handler;
	new->error_handler = error_handler;
	new->finish_handler = finish_handler;
	new->fd = -1;
	new->ref = 1;
	new->finished = FALSE;

	/* Init priv */
	priv = g_slice_alloc0 (sizeof (struct rspamd_http_connection_private));
	new->priv = priv;

	rspamd_http_parser_reset (new);
	priv->parser.data = new;

	return new;
}

void
rspamd_http_connection_reset (struct rspamd_http_connection *conn)
{
	struct rspamd_http_connection_private *priv;
	struct rspamd_http_message *msg;

	priv = conn->priv;
	msg = priv->msg;

	/* Clear request */
	if (msg != NULL) {
		rspamd_http_message_free (msg);
		priv->msg = NULL;
	}
	conn->finished = FALSE;
	/* Clear priv */
	event_del (&priv->ev);
	if (priv->buf != NULL) {
		REF_RELEASE (priv->buf);
	}

	rspamd_http_parser_reset (conn);

	if (priv->out != NULL) {
		g_slice_free1 (sizeof (struct iovec) * priv->outlen, priv->out);
		priv->out = NULL;
	}
}

void
rspamd_http_connection_free (struct rspamd_http_connection *conn)
{
	struct rspamd_http_connection_private *priv;

	priv = conn->priv;
	rspamd_http_connection_reset (conn);
	g_slice_free1 (sizeof (struct rspamd_http_connection_private), priv);
	g_slice_free1 (sizeof (struct rspamd_http_connection),		   conn);
}

void
rspamd_http_connection_read_message (struct rspamd_http_connection *conn,
	gpointer ud, gint fd, struct timeval *timeout, struct event_base *base)
{
	struct rspamd_http_connection_private *priv = conn->priv;
	struct rspamd_http_message *req;

	conn->fd = fd;
	conn->ud = ud;
	req = rspamd_http_new_message (
		conn->type == RSPAMD_HTTP_SERVER ? HTTP_REQUEST : HTTP_RESPONSE);
	priv->msg = req;

	if (timeout == NULL) {
		priv->ptv = NULL;
	}
	else {
		memcpy (&priv->tv, timeout, sizeof (struct timeval));
		priv->ptv = &priv->tv;
	}
	priv->header = NULL;
	priv->buf = g_slice_alloc0 (sizeof (*priv->buf));
	REF_INIT_RETAIN (priv->buf, rspamd_http_privbuf_dtor);
	priv->buf->data = g_string_sized_new (BUFSIZ);
	priv->new_header = TRUE;

	event_set (&priv->ev,
		fd,
		EV_READ | EV_PERSIST,
		rspamd_http_event_handler,
		conn);
	if (base != NULL) {
		event_base_set (base, &priv->ev);
	}
	event_add (&priv->ev, priv->ptv);
}

void
rspamd_http_connection_write_message (struct rspamd_http_connection *conn,
	struct rspamd_http_message *msg, const gchar *host, const gchar *mime_type,
	gpointer ud, gint fd, struct timeval *timeout, struct event_base *base)
{
	struct rspamd_http_connection_private *priv = conn->priv;
	struct rspamd_http_header *hdr;
	struct tm t, *ptm;
	gchar datebuf[64], *pbody;
	gint i;
	gsize bodylen;
	GString *buf;

	conn->fd = fd;
	conn->ud = ud;
	priv->msg = msg;

	if (timeout == NULL) {
		priv->ptv = NULL;
	}
	else {
		memcpy (&priv->tv, timeout, sizeof (struct timeval));
		priv->ptv = &priv->tv;
	}
	priv->header = NULL;
	priv->buf = g_slice_alloc0 (sizeof (*priv->buf));
	REF_INIT_RETAIN (priv->buf, rspamd_http_privbuf_dtor);
	priv->buf->data = g_string_sized_new (128);
	buf = priv->buf->data;

	if (msg->method < HTTP_SYMBOLS) {
		if (msg->body == NULL || msg->body->len == 0) {
			pbody = NULL;
			bodylen = 0;
			priv->outlen = 2;
			msg->method = HTTP_GET;
		}
		else {
			pbody = msg->body->str;
			bodylen = msg->body->len;
			priv->outlen = 3;
			msg->method = HTTP_POST;
		}
	}
	else if (msg->body != NULL) {
		pbody = msg->body->str;
		bodylen = msg->body->len;
		priv->outlen = 2;
	}
	else {
		/* Invalid body for spamc method */
		return;
	}

	if (conn->type == RSPAMD_HTTP_SERVER) {
		/* Format reply */
		if (msg->method < HTTP_SYMBOLS) {
			ptm = gmtime (&msg->date);
			t = *ptm;
			rspamd_snprintf (datebuf,
				sizeof (datebuf),
				"%s, %02d %s %4d %02d:%02d:%02d GMT",
				http_week[t.tm_wday],
				t.tm_mday,
				http_month[t.tm_mon],
				t.tm_year + 1900,
				t.tm_hour,
				t.tm_min,
				t.tm_sec);
			if (mime_type == NULL) {
				mime_type = "text/plain";
			}
			rspamd_printf_gstring (buf, "HTTP/1.1 %d %s\r\n"
				"Connection: close\r\n"
				"Server: %s\r\n"
				"Date: %s\r\n"
				"Content-Length: %z\r\n"
				"Content-Type: %s\r\n",
				msg->code,
				msg->status ? msg->status->str : rspamd_http_code_to_str (msg->
				code),
				"rspamd/" RVERSION,
				datebuf,
				bodylen,
				mime_type);
		}
		else {
			/* Legacy spamd reply */
			rspamd_printf_gstring (buf, "RSPAMD/1.3 0 EX_OK\r\n");
		}
	}
	else {
		/* Format request */
		if (host == NULL && msg->host == NULL) {
			/* Fallback to HTTP/1.0 */
			rspamd_printf_gstring (buf, "%s %v HTTP/1.0\r\n"
					"Content-Length: %z\r\n",
					http_method_str (msg->method), msg->url, bodylen);
		}
		else {
			rspamd_printf_gstring (buf, "%s %v HTTP/1.1\r\n"
				"Connection: close\r\n"
				"Host: %s\r\n"
				"Content-Length: %z\r\n",
				http_method_str (msg->method), msg->url,
				host != NULL ? host : msg->host->str,
				bodylen);
		}
	}
	/* Allocate iov */
	priv->wr_total = bodylen + buf->len + 2;
	DL_FOREACH (msg->headers, hdr)
	{
		/* <name><: ><value><\r\n> */
		priv->wr_total += hdr->name->len + hdr->value->len + 4;
		priv->outlen += 4;
	}
	priv->out = g_slice_alloc (sizeof (struct iovec) * priv->outlen);
	priv->wr_pos = 0;

	/* Now set up all iov */
	priv->out[0].iov_base = buf->str;
	priv->out[0].iov_len = buf->len;
	i = 1;
	LL_FOREACH (msg->headers, hdr)
	{
		priv->out[i].iov_base = hdr->name->str;
		priv->out[i++].iov_len = hdr->name->len;
		priv->out[i].iov_base = ": ";
		priv->out[i++].iov_len = 2;
		priv->out[i].iov_base = hdr->value->str;
		priv->out[i++].iov_len = hdr->value->len;
		priv->out[i].iov_base = "\r\n";
		priv->out[i++].iov_len = 2;
	}
	if (msg->method < HTTP_SYMBOLS) {
		priv->out[i].iov_base = "\r\n";
		priv->out[i++].iov_len = 2;
	}
	else {
		/* No CRLF for compatibility reply */
		priv->wr_total -= 2;
	}
	if (msg->body != NULL) {
		priv->out[i].iov_base = pbody;
		priv->out[i++].iov_len = bodylen;
	}

	event_set (&priv->ev, fd, EV_WRITE, rspamd_http_event_handler, conn);
	if (base != NULL) {
		event_base_set (base, &priv->ev);
	}
	event_add (&priv->ev, priv->ptv);
}

struct rspamd_http_message *
rspamd_http_new_message (enum http_parser_type type)
{
	struct rspamd_http_message *new;

	new = g_slice_alloc (sizeof (struct rspamd_http_message));
	if (type == HTTP_REQUEST) {
		new->url = g_string_sized_new (32);
	}
	else {
		new->url = NULL;
		new->code = 200;
	}
	new->headers = NULL;
	new->date = 0;
	new->body = NULL;
	new->status = NULL;
	new->host = NULL;
	new->port = 80;
	new->type = type;
	new->method = HTTP_GET;

	return new;
}

struct rspamd_http_message*
rspamd_http_message_from_url (const gchar *url)
{
	struct http_parser_url pu;
	struct rspamd_http_message *msg;
	const gchar *host, *path;
	size_t pathlen;

	if (url == NULL) {
		return NULL;
	}

	memset (&pu, 0, sizeof (pu));
	if (http_parser_parse_url (url, strlen (url), FALSE, &pu) != 0) {
		msg_warn ("cannot parse URL: %s", url);
		return NULL;
	}

	if ((pu.field_set & (1 << UF_HOST)) == 0) {
		msg_warn ("no host argument in URL: %s", url);
		return NULL;
	}
	if ((pu.field_set & (1 << UF_PATH)) == 0) {
		path = "/";
		pathlen = 1;
	}
	else {
		path = url + pu.field_data[UF_PATH].off;
		pathlen = pu.field_data[UF_PATH].len;
	}

	msg = rspamd_http_new_message (HTTP_REQUEST);
	host = url + pu.field_data[UF_HOST].off;

	if ((pu.field_set & (1 << UF_PORT)) != 0) {
		msg->port = pu.port;
	}
	else {
		/* XXX: magic constant */
		msg->port = 80;
	}

	msg->host = g_string_new_len (host, pu.field_data[UF_HOST].len);
	g_string_append_len (msg->url, path, pathlen);

	return msg;
}

void
rspamd_http_message_free (struct rspamd_http_message *msg)
{
	struct rspamd_http_header *hdr, *tmp_hdr;

	LL_FOREACH_SAFE (msg->headers, hdr, tmp_hdr)
	{
		g_string_free (hdr->name,  TRUE);
		g_string_free (hdr->value, TRUE);
		g_slice_free1 (sizeof (struct rspamd_http_header), hdr);
	}
	if (msg->body != NULL) {
		g_string_free (msg->body, TRUE);
	}
	if (msg->url != NULL) {
		g_string_free (msg->url, TRUE);
	}
	if (msg->status != NULL) {
		g_string_free (msg->status, TRUE);
	}
	g_slice_free1 (sizeof (struct rspamd_http_message), msg);
}

void
rspamd_http_message_add_header (struct rspamd_http_message *msg,
	const gchar *name,
	const gchar *value)
{
	struct rspamd_http_header *hdr;

	if (msg != NULL && name != NULL && value != NULL) {
		hdr = g_slice_alloc (sizeof (struct rspamd_http_header));
		hdr->name = g_string_new (name);
		hdr->value = g_string_new (value);
		DL_APPEND (msg->headers, hdr);
	}
}

const gchar *
rspamd_http_message_find_header (struct rspamd_http_message *msg,
	const gchar *name)
{
	struct rspamd_http_header *hdr;
	const gchar *res = NULL;
	guint slen = strlen (name);

	if (msg != NULL) {
		LL_FOREACH (msg->headers, hdr)
		{
			if (hdr->name->len == slen) {
				if (g_ascii_strncasecmp(hdr->name->str, name, slen) == 0) {
					res = hdr->value->str;
					break;
				}
			}
		}
	}

	return res;
}

/*
 * HTTP router functions
 */

static void
rspamd_http_entry_free (struct rspamd_http_connection_entry *entry)
{
	if (entry != NULL) {
		close (entry->conn->fd);
		rspamd_http_connection_unref (entry->conn);
		if (entry->rt->finish_handler) {
			entry->rt->finish_handler (entry);
		}
		g_slice_free1 (sizeof (struct rspamd_http_connection_entry), entry);
	}
}

static void
rspamd_http_router_error_handler (struct rspamd_http_connection *conn,
	GError *err)
{
	struct rspamd_http_connection_entry *entry = conn->ud;
	struct rspamd_http_message *msg;

	if (entry->is_reply) {
		/* At this point we need to finish this session and close owned socket */
		if (entry->rt->error_handler != NULL) {
			entry->rt->error_handler (entry, err);
		}
		rspamd_http_entry_free (entry);
	}
	else {
		/* Here we can write a reply to a client */
		if (entry->rt->error_handler != NULL) {
			entry->rt->error_handler (entry, err);
		}
		msg = rspamd_http_new_message (HTTP_RESPONSE);
		msg->date = time (NULL);
		msg->code = err->code;
		msg->body = g_string_new (err->message);
		rspamd_http_connection_reset (entry->conn);
		rspamd_http_connection_write_message (entry->conn,
			msg,
			NULL,
			"text/plain",
			entry,
			entry->conn->fd,
			entry->rt->ptv,
			entry->rt->ev_base);
		entry->is_reply = TRUE;
	}
}

static const gchar *
rspamd_http_router_detect_ct (const gchar *path)
{
	const gchar *dot;
	guint i;

	dot = strrchr (path, '.');
	if (dot == NULL) {
		return http_file_types[HTTP_MAGIC_PLAIN].ct;
	}
	dot++;

	for (i = 0; i < G_N_ELEMENTS (http_file_types); i++) {
		if (strcmp (http_file_types[i].ext, dot) == 0) {
			return http_file_types[i].ct;
		}
	}

	return http_file_types[HTTP_MAGIC_PLAIN].ct;
}

static gboolean
rspamd_http_router_is_subdir (const gchar *parent, const gchar *sub)
{
	if (parent == NULL || sub == NULL || *parent == '\0') {
		return FALSE;
	}

	while (*parent != '\0') {
		if (*sub != *parent) {
			return FALSE;
		}
		parent++;
		sub++;
	}

	parent--;
	if (*parent == G_DIR_SEPARATOR) {
		return TRUE;
	}

	return (*sub == G_DIR_SEPARATOR || *sub == '\0');
}

static gboolean
rspamd_http_router_try_file (struct rspamd_http_connection_entry *entry,
	struct rspamd_http_message *msg, gboolean expand_path)
{
	struct stat st;
	gint fd;
	gchar filebuf[PATH_MAX], realbuf[PATH_MAX], *dir;
	struct rspamd_http_message *reply_msg;

	rspamd_snprintf (filebuf, sizeof (filebuf), "%s%c%v",
		entry->rt->default_fs_path, G_DIR_SEPARATOR, msg->url);

	if (realpath (filebuf, realbuf) == NULL ||
		lstat (realbuf, &st) == -1) {
		return FALSE;
	}

	if (S_ISDIR (st.st_mode) && expand_path) {
		/* Try to append 'index.html' to the url */
		g_string_append_printf (msg->url, "%c%s", G_DIR_SEPARATOR,
			"index.html");
		return rspamd_http_router_try_file (entry, msg, FALSE);
	}
	else if (!S_ISREG (st.st_mode)) {
		return FALSE;
	}

	/* We also need to ensure that file is inside the defined dir */
	rspamd_strlcpy (filebuf, realbuf, sizeof (filebuf));
	dir = dirname (filebuf);
	if (dir == NULL ||
		!rspamd_http_router_is_subdir (entry->rt->default_fs_path,
		dir)) {
		return FALSE;
	}

	fd = open (realbuf, O_RDONLY);
	if (fd == -1) {
		return FALSE;
	}

	reply_msg = rspamd_http_new_message (HTTP_RESPONSE);
	reply_msg->date = time (NULL);
	reply_msg->code = 200;
	reply_msg->body = g_string_sized_new (st.st_size);

	if (read (fd, reply_msg->body->str, st.st_size) != st.st_size) {
		close (fd);
		rspamd_http_message_free (reply_msg);
		return FALSE;
	}

	reply_msg->body->len = st.st_size;
	reply_msg->body->str[st.st_size] = '\0';
	close (fd);

	rspamd_http_connection_reset (entry->conn);

	msg_debug ("requested file %s", realbuf);
	rspamd_http_connection_write_message (entry->conn, reply_msg, NULL,
		rspamd_http_router_detect_ct (realbuf), entry, entry->conn->fd,
		entry->rt->ptv, entry->rt->ev_base);

	return TRUE;
}

static int
rspamd_http_router_finish_handler (struct rspamd_http_connection *conn,
	struct rspamd_http_message *msg)
{
	struct rspamd_http_connection_entry *entry = conn->ud;
	rspamd_http_router_handler_t handler = NULL;
	gpointer found;
	struct rspamd_http_message *err_msg;
	GError *err;

	G_STATIC_ASSERT (sizeof (rspamd_http_router_handler_t) ==
		sizeof (gpointer));

	if (entry->is_reply) {
		/* Request is finished, it is safe to free a connection */
		rspamd_http_entry_free (entry);
	}
	else {
		/* Search for path */
		if (msg->url != NULL && msg->url->len != 0) {
			found = g_hash_table_lookup (entry->rt->paths, msg->url->str);
			memcpy (&handler, &found, sizeof (found));
			msg_debug ("requested known path: %v", msg->url);
		}
		entry->is_reply = TRUE;
		if (handler != NULL) {
			return handler (entry, msg);
		}
		else {
			if (entry->rt->default_fs_path == NULL ||
				!rspamd_http_router_try_file (entry, msg, TRUE)) {
				err = g_error_new (HTTP_ERROR, 404,
						"Not found");
				if (entry->rt->error_handler != NULL) {
					entry->rt->error_handler (entry, err);
				}
				msg_info ("path: %v not found", msg->url);
				err_msg = rspamd_http_new_message (HTTP_RESPONSE);
				err_msg->date = time (NULL);
				err_msg->code = err->code;
				err_msg->body = g_string_new (err->message);
				rspamd_http_connection_reset (entry->conn);
				rspamd_http_connection_write_message (entry->conn,
					err_msg,
					NULL,
					"text/plain",
					entry,
					entry->conn->fd,
					entry->rt->ptv,
					entry->rt->ev_base);
				g_error_free (err);
			}
		}
	}

	return 0;
}

struct rspamd_http_connection_router *
rspamd_http_router_new (rspamd_http_router_error_handler_t eh,
	rspamd_http_router_finish_handler_t fh,
	struct timeval *timeout, struct event_base *base,
	const char *default_fs_path)
{
	struct rspamd_http_connection_router * new;
	struct stat st;

	new = g_slice_alloc (sizeof (struct rspamd_http_connection_router));
	new->paths = g_hash_table_new (rspamd_strcase_hash, rspamd_strcase_equal);
	new->conns = NULL;
	new->error_handler = eh;
	new->finish_handler = fh;
	new->ev_base = base;
	if (timeout) {
		new->tv = *timeout;
		new->ptv = &new->tv;
	}
	else {
		new->ptv = NULL;
	}

	new->default_fs_path = NULL;
	if (default_fs_path != NULL) {
		if (stat (default_fs_path, &st) == -1) {
			msg_err ("cannot stat %s", default_fs_path);
		}
		else {
			if (!S_ISDIR (st.st_mode)) {
				msg_err ("path %s is not a directory", default_fs_path);
			}
			else {
				new->default_fs_path = realpath (default_fs_path, NULL);
			}
		}
	}

	return new;
}

void
rspamd_http_router_add_path (struct rspamd_http_connection_router *router,
	const gchar *path, rspamd_http_router_handler_t handler)
{
	gpointer ptr;
	G_STATIC_ASSERT (sizeof (rspamd_http_router_handler_t) ==
		sizeof (gpointer));

	if (path != NULL && handler != NULL && router != NULL) {
		memcpy (&ptr, &handler, sizeof (ptr));
		g_hash_table_insert (router->paths, (gpointer)path, ptr);
	}
}

void
rspamd_http_router_handle_socket (struct rspamd_http_connection_router *router,
	gint fd, gpointer ud)
{
	struct rspamd_http_connection_entry *conn;

	conn = g_slice_alloc (sizeof (struct rspamd_http_connection_entry));
	conn->rt = router;
	conn->ud = ud;
	conn->is_reply = FALSE;

	conn->conn = rspamd_http_connection_new (NULL,
			rspamd_http_router_error_handler,
			rspamd_http_router_finish_handler,
			0,
			RSPAMD_HTTP_SERVER);

	rspamd_http_connection_read_message (conn->conn, conn, fd, router->ptv,
		router->ev_base);
	LL_PREPEND (router->conns, conn);
}

void
rspamd_http_router_free (struct rspamd_http_connection_router *router)
{
	struct rspamd_http_connection_entry *conn, *tmp;

	if (router) {
		LL_FOREACH_SAFE (router->conns, conn, tmp)
		{
			rspamd_http_entry_free (conn);
		}

		if (router->default_fs_path != NULL) {
			g_free (router->default_fs_path);
		}
		g_hash_table_unref (router->paths);
		g_slice_free1 (sizeof (struct rspamd_http_connection_router), router);
	}
}
