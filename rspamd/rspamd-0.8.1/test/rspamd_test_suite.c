#include "config.h"
#include "main.h"
#include "cfg_file.h"
#include "tests.h"

struct rspamd_main             *rspamd_main = NULL;
struct event_base              *base = NULL;
worker_t *workers[] = { NULL };


int
main (int argc, char **argv)
{
	struct rspamd_config            *cfg;

	g_test_init (&argc, &argv, NULL);

	rspamd_main = (struct rspamd_main *)g_malloc (sizeof (struct rspamd_main));

#if ((GLIB_MAJOR_VERSION == 2) && (GLIB_MINOR_VERSION <= 30))
	g_thread_init (NULL);
#endif

	g_mime_init (0);
	memset (rspamd_main, 0, sizeof (struct rspamd_main));
	rspamd_main->server_pool = rspamd_mempool_new (rspamd_mempool_suggest_size ());
	rspamd_main->cfg = (struct rspamd_config *)g_malloc (sizeof (struct rspamd_config));
	cfg = rspamd_main->cfg;
	bzero (cfg, sizeof (struct rspamd_config));
	cfg->cfg_pool = rspamd_mempool_new (rspamd_mempool_suggest_size ());

	base = event_init ();

	if (g_test_verbose ()) {
		cfg->log_level = G_LOG_LEVEL_DEBUG;
	}
	else {
		cfg->log_level = G_LOG_LEVEL_INFO;
	}
	cfg->log_type = RSPAMD_LOG_CONSOLE;
	/* First set logger to console logger */
	rspamd_set_logger (cfg, g_quark_from_static_string("rspamd-test"), rspamd_main);
	(void)rspamd_log_open (rspamd_main->logger);
	g_log_set_default_handler (rspamd_glib_log_function, rspamd_main->logger);

	g_test_add_func ("/rspamd/mem_pool", rspamd_mem_pool_test_func);
	g_test_add_func ("/rspamd/fuzzy", rspamd_fuzzy_test_func);
	g_test_add_func ("/rspamd/url", rspamd_url_test_func);
	g_test_add_func ("/rspamd/expression", rspamd_expression_test_func);
	g_test_add_func ("/rspamd/statfile", rspamd_statfile_test_func);
	g_test_add_func ("/rspamd/radix", rspamd_radix_test_func);
	g_test_add_func ("/rspamd/dns", rspamd_dns_test_func);
	g_test_add_func ("/rspamd/aio", rspamd_async_test_func);
	g_test_add_func ("/rspamd/dkim", rspamd_dkim_test_func);
	g_test_add_func ("/rspamd/rrd", rspamd_rrd_test_func);
	g_test_add_func ("/rspamd/upstream", rspamd_upstream_test_func);
	g_test_add_func ("/rspamd/shingles", rspamd_shingles_test_func);

	g_test_run ();

	g_mime_shutdown ();

	return 0;
}
