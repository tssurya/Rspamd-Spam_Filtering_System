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

#include "cfg_file.h"
#include "main.h"
#include "uthash_strcase.h"
#include "filter.h"
#include "classifiers/classifiers.h"
#include "lua/lua_common.h"
#include "kvstorage_config.h"
#include "map.h"
#include "dynamic_cfg.h"
#include "utlist.h"

#define DEFAULT_SCORE 10.0

#define DEFAULT_RLIMIT_NOFILE 2048
#define DEFAULT_RLIMIT_MAXCORE 0
#define DEFAULT_MAP_TIMEOUT 10

struct rspamd_ucl_map_cbdata {
	struct rspamd_config *cfg;
	GString *buf;
};
static gchar * rspamd_ucl_read_cb (rspamd_mempool_t * pool,
	gchar * chunk,
	gint len,
	struct map_cb_data *data);
static void rspamd_ucl_fin_cb (rspamd_mempool_t * pool,
	struct map_cb_data *data);

gboolean
rspamd_parse_bind_line (struct rspamd_config *cfg,
	struct rspamd_worker_conf *cf,
	const gchar *str)
{
	struct rspamd_worker_bind_conf *cnf;
	gchar **tokens, *err;
	gboolean ret = TRUE;

	if (str == NULL) {
		return FALSE;
	}

	if (str[0] == '[') {
		/* This is an ipv6 address */
		gsize len, ntok;
		const gchar *start, *ip_pos;

		start = str + 1;

		len = strcspn (start, "]");
		if (start[len] != ']') {
			return FALSE;
		}

		ip_pos = start;
		start += len + 1;
		ntok = 1;

		if (*start == ':') {
			ntok = 2;
			start ++;
		}

		tokens = g_malloc_n (ntok + 1, sizeof (gchar *));
		tokens[ntok] = NULL;
		tokens[0] = g_malloc (len + 1);
		rspamd_strlcpy (tokens[0], ip_pos, len + 1);

		if (ntok > 1) {
			tokens[1] = g_strdup (start);
		}
	}
	else {
		tokens = g_strsplit_set (str, ":", 0);
	}
	if (!tokens || !tokens[0]) {
		return FALSE;
	}

	cnf =
		rspamd_mempool_alloc0 (cfg->cfg_pool,
			sizeof (struct rspamd_worker_bind_conf));

	cnf->cnt = 1024;
	if (strcmp (tokens[0], "systemd") == 0) {
		/* The actual socket will be passed by systemd environment */
		cnf->is_systemd = TRUE;
		cnf->cnt = strtoul (tokens[1], &err, 10);
		cnf->addrs = NULL;
		if (err == NULL || *err == '\0') {
			LL_PREPEND (cf->bind_conf, cnf);
		}
		else {
			msg_err ("cannot parse bind line: %s", str);
			ret = FALSE;
		}
	}
	else {
		if (!rspamd_parse_host_port_priority_strv (tokens, &cnf->addrs,
				&cnf->cnt, NULL, &cnf->name, DEFAULT_BIND_PORT, cfg->cfg_pool)) {
			msg_err ("cannot parse bind line: %s", str);
			ret = FALSE;
		}
		else {
			LL_PREPEND (cf->bind_conf, cnf);
		}
	}

	g_strfreev (tokens);

	return ret;
}

void
rspamd_config_defaults (struct rspamd_config *cfg)
{
	cfg->dns_timeout = 1000;
	cfg->dns_retransmits = 5;
	/* After 20 errors do throttling for 10 seconds */
	cfg->dns_throttling_errors = 20;
	cfg->dns_throttling_time = 10000;
	/* 16 sockets per DNS server */
	cfg->dns_io_per_server = 16;

	cfg->statfile_sync_interval = 60000;
	cfg->statfile_sync_timeout = 20000;

	/* 20 Kb */
	cfg->max_diff = 20480;

	cfg->metrics = g_hash_table_new (rspamd_str_hash, rspamd_str_equal);
	if (cfg->c_modules == NULL) {
		cfg->c_modules = g_hash_table_new (rspamd_str_hash, rspamd_str_equal);
	}
	cfg->composite_symbols =
		g_hash_table_new (rspamd_str_hash, rspamd_str_equal);
	cfg->classifiers_symbols = g_hash_table_new (rspamd_str_hash,
			rspamd_str_equal);
	cfg->cfg_params = g_hash_table_new (rspamd_str_hash, rspamd_str_equal);
	cfg->metrics_symbols = g_hash_table_new (rspamd_str_hash, rspamd_str_equal);

	cfg->map_timeout = DEFAULT_MAP_TIMEOUT;

	cfg->log_level = G_LOG_LEVEL_WARNING;
	cfg->log_extended = TRUE;
}

void
rspamd_config_free (struct rspamd_config *cfg)
{
	GList *cur;
	struct rspamd_symbols_group *gr;

	rspamd_map_remove_all (cfg);
	ucl_obj_unref (cfg->rcl_obj);
	g_hash_table_remove_all (cfg->metrics);
	g_hash_table_unref (cfg->metrics);
	g_hash_table_unref (cfg->c_modules);
	g_hash_table_remove_all (cfg->composite_symbols);
	g_hash_table_unref (cfg->composite_symbols);
	g_hash_table_remove_all (cfg->cfg_params);
	g_hash_table_unref (cfg->cfg_params);
	g_hash_table_destroy (cfg->metrics_symbols);
	g_hash_table_destroy (cfg->classifiers_symbols);
	/* Free symbols groups */
	cur = cfg->symbols_groups;
	while (cur) {
		gr = cur->data;
		if (gr->symbols) {
			g_list_free (gr->symbols);
		}
		cur = g_list_next (cur);
	}
	if (cfg->symbols_groups) {
		g_list_free (cfg->symbols_groups);
	}

	if (cfg->checksum) {
		g_free (cfg->checksum);
	}
	g_list_free (cfg->classifiers);
	g_list_free (cfg->metrics_list);
	rspamd_mempool_delete (cfg->cfg_pool);
}

const ucl_object_t *
rspamd_config_get_module_opt (struct rspamd_config *cfg,
	const gchar *module_name,
	const gchar *opt_name)
{
	const ucl_object_t *res = NULL, *sec;

	sec = ucl_obj_get_key (cfg->rcl_obj, module_name);
	if (sec != NULL) {
		res = ucl_obj_get_key (sec, opt_name);
	}

	return res;
}

guint64
rspamd_config_parse_limit (const gchar *limit, guint len)
{
	guint64 result = 0;
	const gchar *err_str;

	if (!limit || *limit == '\0' || len == 0) {
		return 0;
	}

	errno = 0;
	result = strtoull (limit, (gchar **)&err_str, 10);

	if (*err_str != '\0') {
		/* Megabytes */
		if (*err_str == 'm' || *err_str == 'M') {
			result *= 1048576L;
		}
		/* Kilobytes */
		else if (*err_str == 'k' || *err_str == 'K') {
			result *= 1024;
		}
		/* Gigabytes */
		else if (*err_str == 'g' || *err_str == 'G') {
			result *= 1073741824L;
		}
		else if (len > 0 && err_str - limit != (gint)len) {
			msg_warn ("invalid limit value '%s' at position '%s'",
				limit,
				err_str);
			result = 0;
		}
	}

	return result;
}

gchar
rspamd_config_parse_flag (const gchar *str)
{
	guint len;
	gchar c;

	if (!str || !*str) {
		return -1;
	}

	len = strlen (str);

	switch (len) {
	case 1:
		c = g_ascii_tolower (*str);
		if (c == 'y' || c == '1') {
			return 1;
		}
		else if (c == 'n' || c == '0') {
			return 0;
		}
		break;
	case 2:
		if (g_ascii_strncasecmp (str, "no", len) == 0) {
			return 0;
		}
		else if (g_ascii_strncasecmp (str, "on", len) == 0) {
			return 1;
		}
		break;
	case 3:
		if (g_ascii_strncasecmp (str, "yes", len) == 0) {
			return 1;
		}
		else if (g_ascii_strncasecmp (str, "off", len) == 0) {
			return 0;
		}
		break;
	case 4:
		if (g_ascii_strncasecmp (str, "true", len) == 0) {
			return 1;
		}
		break;
	case 5:
		if (g_ascii_strncasecmp (str, "false", len) == 0) {
			return 0;
		}
		break;
	}

	return -1;
}

gboolean
rspamd_config_calculate_checksum (struct rspamd_config *cfg)
{
	gint fd;
	void *map;
	struct stat st;

	/* Compute checksum for config file that should be used by xml dumper */
	if ((fd = open (cfg->cfg_name, O_RDONLY)) == -1) {
		msg_err (
			"config file %s is no longer available, cannot calculate checksum");
		return FALSE;
	}
	if (stat (cfg->cfg_name, &st) == -1) {
		msg_err ("cannot stat %s: %s", cfg->cfg_name, strerror (errno));
		return FALSE;
	}

	/* Now mmap this file to simplify reading process */
	if ((map =
		mmap (NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		msg_err ("cannot mmap %s: %s", cfg->cfg_name, strerror (errno));
		close (fd);
		return FALSE;
	}
	close (fd);

	/* Get checksum for a file */
	cfg->checksum = g_compute_checksum_for_string (G_CHECKSUM_MD5,
			map,
			st.st_size);
	munmap (map, st.st_size);

	return TRUE;
}
/*
 * Perform post load actions
 */
void
rspamd_config_post_load (struct rspamd_config *cfg)
{
#ifdef HAVE_CLOCK_GETTIME
	struct timespec ts;
#endif
	struct metric *def_metric;

#ifdef HAVE_CLOCK_GETTIME
#ifdef HAVE_CLOCK_PROCESS_CPUTIME_ID
	clock_getres (CLOCK_PROCESS_CPUTIME_ID, &ts);
# elif defined(HAVE_CLOCK_VIRTUAL)
	clock_getres (CLOCK_VIRTUAL,			&ts);
# else
	clock_getres (CLOCK_REALTIME,			&ts);
# endif

	cfg->clock_res = (gint)log10 (1000000 / ts.tv_nsec);
	if (cfg->clock_res < 0) {
		cfg->clock_res = 0;
	}
	if (cfg->clock_res > 3) {
		cfg->clock_res = 3;
	}
#else
	/* For gettimeofday */
	cfg->clock_res = 1;
#endif

	if ((def_metric =
		g_hash_table_lookup (cfg->metrics, DEFAULT_METRIC)) == NULL) {
		def_metric = rspamd_config_new_metric (cfg, NULL);
		def_metric->name = DEFAULT_METRIC;
		def_metric->actions[METRIC_ACTION_REJECT].score = DEFAULT_SCORE;
		cfg->metrics_list = g_list_prepend (cfg->metrics_list, def_metric);
		g_hash_table_insert (cfg->metrics, DEFAULT_METRIC, def_metric);
	}

	cfg->default_metric = def_metric;

	/* Lua options */
	(void)rspamd_lua_post_load_config (cfg);
	init_dynamic_config (cfg);
}

#if 0
void
parse_err (const gchar *fmt, ...)
{
	va_list aq;
	gchar logbuf[BUFSIZ], readbuf[32];
	gint r;

	va_start (aq, fmt);
	rspamd_strlcpy (readbuf, yytext, sizeof (readbuf));

	r = snprintf (logbuf,
			sizeof (logbuf),
			"config file parse error! line: %d, text: %s, reason: ",
			yylineno,
			readbuf);
	r += vsnprintf (logbuf + r, sizeof (logbuf) - r, fmt, aq);

	va_end (aq);
	g_critical ("%s", logbuf);
}

void
parse_warn (const gchar *fmt, ...)
{
	va_list aq;
	gchar logbuf[BUFSIZ], readbuf[32];
	gint r;

	va_start (aq, fmt);
	rspamd_strlcpy (readbuf, yytext, sizeof (readbuf));

	r = snprintf (logbuf,
			sizeof (logbuf),
			"config file parse warning! line: %d, text: %s, reason: ",
			yylineno,
			readbuf);
	r += vsnprintf (logbuf + r, sizeof (logbuf) - r, fmt, aq);

	va_end (aq);
	g_warning ("%s", logbuf);
}
#endif

void
rspamd_config_unescape_quotes (gchar *line)
{
	gchar *c = line, *t;

	while (*c) {
		if (*c == '\\' && *(c + 1) == '"') {
			t = c;
			while (*t) {
				*t = *(t + 1);
				t++;
			}
		}
		c++;
	}
}

GList *
rspamd_config_parse_comma_list (rspamd_mempool_t * pool, const gchar *line)
{
	GList *res = NULL;
	const gchar *c, *p;
	gchar *str;

	c = line;
	p = c;

	while (*p) {
		if (*p == ',' && *c != *p) {
			str = rspamd_mempool_alloc (pool, p - c + 1);
			rspamd_strlcpy (str, c, p - c + 1);
			res = g_list_prepend (res, str);
			/* Skip spaces */
			while (g_ascii_isspace (*(++p))) ;
			c = p;
			continue;
		}
		p++;
	}
	if (res != NULL) {
		rspamd_mempool_add_destructor (pool,
			(rspamd_mempool_destruct_t) g_list_free,
			res);
	}

	return res;
}

struct rspamd_classifier_config *
rspamd_config_new_classifier (struct rspamd_config *cfg,
	struct rspamd_classifier_config *c)
{
	if (c == NULL) {
		c =
			rspamd_mempool_alloc0 (cfg->cfg_pool,
				sizeof (struct rspamd_classifier_config));
	}
	if (c->opts == NULL) {
		c->opts = g_hash_table_new (rspamd_str_hash, rspamd_str_equal);
		rspamd_mempool_add_destructor (cfg->cfg_pool,
			(rspamd_mempool_destruct_t) g_hash_table_destroy,
			c->opts);
	}
	if (c->labels == NULL) {
		c->labels = g_hash_table_new_full (rspamd_str_hash,
				rspamd_str_equal,
				NULL,
				(GDestroyNotify)g_list_free);
		rspamd_mempool_add_destructor (cfg->cfg_pool,
			(rspamd_mempool_destruct_t) g_hash_table_destroy,
			c->labels);
	}

	return c;
}

struct rspamd_statfile_config *
rspamd_config_new_statfile (struct rspamd_config *cfg,
	struct rspamd_statfile_config *c)
{
	if (c == NULL) {
		c =
			rspamd_mempool_alloc0 (cfg->cfg_pool,
				sizeof (struct rspamd_statfile_config));
	}

	return c;
}

struct metric *
rspamd_config_new_metric (struct rspamd_config *cfg, struct metric *c)
{
	int i;
	if (c == NULL) {
		c = rspamd_mempool_alloc0 (cfg->cfg_pool, sizeof (struct metric));
		c->grow_factor = 1.0;
		c->symbols = g_hash_table_new (rspamd_str_hash, rspamd_str_equal);
		c->descriptions = g_hash_table_new (rspamd_str_hash, rspamd_str_equal);
		for (i = METRIC_ACTION_REJECT; i < METRIC_ACTION_MAX; i++) {
			c->actions[i].score = -1.0;
		}
		c->subject = SPAM_SUBJECT;
		rspamd_mempool_add_destructor (cfg->cfg_pool,
			(rspamd_mempool_destruct_t) g_hash_table_destroy,
			c->symbols);
		rspamd_mempool_add_destructor (cfg->cfg_pool,
			(rspamd_mempool_destruct_t) g_hash_table_destroy,
			c->descriptions);
	}

	return c;
}

struct rspamd_worker_conf *
rspamd_config_new_worker (struct rspamd_config *cfg,
	struct rspamd_worker_conf *c)
{
	if (c == NULL) {
		c =
			rspamd_mempool_alloc0 (cfg->cfg_pool,
				sizeof (struct rspamd_worker_conf));
		c->params = g_hash_table_new (rspamd_str_hash, rspamd_str_equal);
		c->active_workers = g_queue_new ();
		rspamd_mempool_add_destructor (cfg->cfg_pool,
			(rspamd_mempool_destruct_t)g_hash_table_destroy,
			c->params);
		rspamd_mempool_add_destructor (cfg->cfg_pool,
			(rspamd_mempool_destruct_t)g_queue_free,
			c->active_workers);
#ifdef HAVE_SC_NPROCESSORS_ONLN
		c->count = sysconf (_SC_NPROCESSORS_ONLN);
#else
		c->count = DEFAULT_WORKERS_NUM;
#endif
		c->rlimit_nofile = 0;
		c->rlimit_maxcore = 0;
	}

	return c;
}


static bool
rspamd_include_map_handler (const guchar *data, gsize len,
		const ucl_object_t *args, void * ud)
{
	struct rspamd_config *cfg = (struct rspamd_config *)ud;
	struct rspamd_ucl_map_cbdata *cbdata, **pcbdata;
	gchar *map_line;

	map_line = rspamd_mempool_alloc (cfg->cfg_pool, len + 1);
	rspamd_strlcpy (map_line, data, len + 1);

	cbdata = g_malloc (sizeof (struct rspamd_ucl_map_cbdata));
	pcbdata = g_malloc (sizeof (struct rspamd_ucl_map_cbdata *));
	cbdata->buf = NULL;
	cbdata->cfg = cfg;
	*pcbdata = cbdata;

	return rspamd_map_add (cfg,
			   map_line,
			   "ucl include",
			   rspamd_ucl_read_cb,
			   rspamd_ucl_fin_cb,
			   (void **)pcbdata);
}

/*
 * Variables:
 * $CONFDIR - configuration directory
 * $RUNDIR - local states directory
 * $DBDIR - databases dir
 * $LOGDIR - logs dir
 * $PLUGINSDIR - pluggins dir
 * $PREFIX - installation prefix
 * $VERSION - rspamd version
 */

#define RSPAMD_CONFDIR_MACRO "CONFDIR"
#define RSPAMD_RUNDIR_MACRO "RUNDIR"
#define RSPAMD_DBDIR_MACRO "DBDIR"
#define RSPAMD_LOGDIR_MACRO "LOGDIR"
#define RSPAMD_PLUGINSDIR_MACRO "PLUGINSDIR"
#define RSPAMD_WWWDIR_MACRO "WWWDIR"
#define RSPAMD_PREFIX_MACRO "PREFIX"
#define RSPAMD_VERSION_MACRO "VERSION"

void
rspamd_ucl_add_conf_variables (struct ucl_parser *parser)
{
	ucl_parser_register_variable (parser,
										 RSPAMD_CONFDIR_MACRO,
																RSPAMD_CONFDIR);
	ucl_parser_register_variable (parser, RSPAMD_RUNDIR_MACRO,
																RSPAMD_RUNDIR);
	ucl_parser_register_variable (parser,  RSPAMD_DBDIR_MACRO,
																RSPAMD_DBDIR);
	ucl_parser_register_variable (parser, RSPAMD_LOGDIR_MACRO,
																RSPAMD_LOGDIR);
	ucl_parser_register_variable (parser,
		RSPAMD_PLUGINSDIR_MACRO,
																RSPAMD_PLUGINSDIR);
	ucl_parser_register_variable (parser,  RSPAMD_WWWDIR_MACRO,
																RSPAMD_WWWDIR);
	ucl_parser_register_variable (parser,  RSPAMD_PREFIX_MACRO,
																RSPAMD_PREFIX);
	ucl_parser_register_variable (parser, RSPAMD_VERSION_MACRO, RVERSION);
}

void
rspamd_ucl_add_conf_macros (struct ucl_parser *parser,
	struct rspamd_config *cfg)
{
	ucl_parser_register_macro (parser,
		"include_map",
		rspamd_include_map_handler,
		cfg);
}

static void
symbols_classifiers_callback (gpointer key, gpointer value, gpointer ud)
{
	struct rspamd_config *cfg = ud;

	register_virtual_symbol (&cfg->cache, key, 1.0);
}

void
rspamd_config_insert_classify_symbols (struct rspamd_config *cfg)
{
	g_hash_table_foreach (cfg->classifiers_symbols,
		symbols_classifiers_callback,
		cfg);
}

struct rspamd_classifier_config *
rspamd_config_find_classifier (struct rspamd_config *cfg, const gchar *name)
{
	GList *cur;
	struct rspamd_classifier_config *cf;

	if (name == NULL) {
		return NULL;
	}

	cur = cfg->classifiers;
	while (cur) {
		cf = cur->data;

		if (g_ascii_strcasecmp (cf->classifier->name, name) == 0) {
			return cf;
		}

		cur = g_list_next (cur);
	}

	return NULL;
}

gboolean
rspamd_config_check_statfiles (struct rspamd_classifier_config *cf)
{
	struct rspamd_statfile_config *st;
	gboolean has_other = FALSE, res = FALSE, cur_class;
	GList *cur;

	/* First check classes directly */
	cur = cf->statfiles;
	while (cur) {
		st = cur->data;
		if (!has_other) {
			cur_class = st->is_spam;
			has_other = TRUE;
		}
		else {
			if (cur_class != st->is_spam) {
				return TRUE;
			}
		}

		cur = g_list_next (cur);
	}

	if (!has_other) {
		/* We have only one statfile */
		return FALSE;
	}
	/* We have not detected any statfile that has different class, so turn on euristic based on symbol's name */
	has_other = FALSE;
	cur = cf->statfiles;
	while (cur) {
		st = cur->data;
		if (rspamd_strncasestr (st->symbol, "spam", -1) != NULL) {
			st->is_spam = TRUE;
		}
		else if (rspamd_strncasestr (st->symbol, "ham", -1) != NULL) {
			st->is_spam = FALSE;
		}

		if (!has_other) {
			cur_class = st->is_spam;
			has_other = TRUE;
		}
		else {
			if (cur_class != st->is_spam) {
				res = TRUE;
			}
		}

		cur = g_list_next (cur);
	}

	return res;
}

static gchar *
rspamd_ucl_read_cb (rspamd_mempool_t * pool,
	gchar * chunk,
	gint len,
	struct map_cb_data *data)
{
	struct rspamd_ucl_map_cbdata *cbdata = data->cur_data, *prev;

	if (cbdata == NULL) {
		cbdata = g_malloc (sizeof (struct rspamd_ucl_map_cbdata));
		prev = data->prev_data;
		cbdata->buf = g_string_sized_new (BUFSIZ);
		cbdata->cfg = prev->cfg;
		data->cur_data = cbdata;
	}
	g_string_append_len (cbdata->buf, chunk, len);

	/* Say not to copy any part of this buffer */
	return NULL;
}

static void
rspamd_ucl_fin_cb (rspamd_mempool_t * pool, struct map_cb_data *data)
{
	struct rspamd_ucl_map_cbdata *cbdata = data->cur_data, *prev =
		data->prev_data;
	ucl_object_t *obj;
	struct ucl_parser *parser;
	guint32 checksum;
	ucl_object_iter_t it = NULL;
	const ucl_object_t *cur;

	if (prev != NULL) {
		if (prev->buf != NULL) {
			g_string_free (prev->buf, TRUE);
		}
		g_free (prev);
	}

	if (cbdata == NULL) {
		msg_err ("map fin error: new data is NULL");
		return;
	}

	checksum = XXH32 (cbdata->buf->str, cbdata->buf->len, 0xdead);
	if (data->map->checksum != checksum) {
		/* New data available */
		parser = ucl_parser_new (0);
		if (!ucl_parser_add_chunk (parser, cbdata->buf->str,
			cbdata->buf->len)) {
			msg_err ("cannot parse map %s: %s",
				data->map->uri,
				ucl_parser_get_error (parser));
			ucl_parser_free (parser);
		}
		else {
			obj = ucl_parser_get_object (parser);
			ucl_parser_free (parser);
			it = NULL;

			while ((cur = ucl_iterate_object (obj, &it, true))) {
				ucl_object_replace_key (cbdata->cfg->rcl_obj, (ucl_object_t *)cur,
						cur->key, cur->keylen, false);
			}
			ucl_object_unref (obj);
			data->map->checksum = checksum;
		}
	}
	else {
		msg_info ("do not reload map %s, checksum is the same: %d",
			data->map->uri,
			checksum);
	}
}

/*
 * vi:ts=4
 */
