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
#ifndef TASK_H_
#define TASK_H_

#include "config.h"
#include "http.h"
#include "events.h"
#include "util.h"
#include "mem_pool.h"
#include "dns.h"

enum rspamd_command {
	CMD_CHECK,
	CMD_SYMBOLS,
	CMD_REPORT,
	CMD_REPORT_IFSPAM,
	CMD_SKIP,
	CMD_PING,
	CMD_PROCESS,
	CMD_OTHER
};

enum rspamd_metric_action {
	METRIC_ACTION_REJECT = 0,
	METRIC_ACTION_SOFT_REJECT,
	METRIC_ACTION_REWRITE_SUBJECT,
	METRIC_ACTION_ADD_HEADER,
	METRIC_ACTION_GREYLIST,
	METRIC_ACTION_NOACTION,
	METRIC_ACTION_MAX
};

typedef gint (*protocol_reply_func)(struct rspamd_task *task);

struct custom_command {
	const gchar *name;
	protocol_reply_func func;
};

/**
 * Worker task structure
 */
struct rspamd_task {
	struct rspamd_worker *worker;                               /**< pointer to worker object						*/
	enum {
		READ_MESSAGE,
		WAIT_PRE_FILTER,
		WAIT_FILTER,
		WAIT_POST_FILTER,
		WRITE_REPLY,
		WRITING_REPLY,
		CLOSING_CONNECTION
	} state;                                                    /**< current session state							*/
	enum rspamd_command cmd;                                    /**< command										*/
	struct custom_command *custom_cmd;                          /**< custom command if any							*/
	gint sock;                                                  /**< socket descriptor								*/
	gboolean is_mime;                                           /**< if this task is mime task                      */
	gboolean is_json;                                           /**< output is JSON									*/
	gboolean skip_extra_filters;                                /**< skip pre and post filters						*/
	gboolean is_skipped;                                        /**< whether message was skipped by configuration   */
	gboolean extended_urls;										/**< output URLs in details							*/

	gchar *helo;                                                    /**< helo header value								*/
	gchar *queue_id;                                                /**< queue id if specified							*/
	const gchar *message_id;                                        /**< message id										*/

	rspamd_inet_addr_t from_addr;                               /**< from addr for a task							*/
	rspamd_inet_addr_t client_addr;                             /**< address of connected socket					*/
	gchar *deliver_to;                                          /**< address to deliver								*/
	gchar *user;                                                    /**< user to deliver								*/
	gchar *subject;                                             /**< subject (for non-mime)							*/
	gchar *hostname;                                            /**< hostname reported by MTA						*/
	GHashTable *request_headers;                                /**< HTTP headers in a request						*/
	GHashTable *reply_headers;                                  /**< Custom reply headers							*/
	GString *msg;                                               /**< message buffer									*/
	struct rspamd_http_connection *http_conn;                   /**< HTTP server connection							*/
	struct rspamd_async_session * s;                             /**< async session object							*/
	gint parts_count;                                           /**< mime parts count								*/
	GMimeMessage *message;                                      /**< message, parsed with GMime						*/
	GMimeObject *parser_parent_part;                            /**< current parent part							*/
	GList *parts;                                               /**< list of parsed parts							*/
	GList *text_parts;                                          /**< list of text parts								*/
	gchar *raw_headers_str;                                         /**< list of raw headers							*/
	GList *received;                                            /**< list of received headers						*/
	GTree *urls;                                                /**< list of parsed urls							*/
	GTree *emails;                                              /**< list of parsed emails							*/
	GList *images;                                              /**< list of images									*/
	GHashTable *raw_headers;                                    /**< list of raw headers							*/
	GHashTable *results;                                        /**< hash table of metric_result indexed by
	                                                             *    metric's name									*/
	GHashTable *tokens;                                         /**< hash table of tokens indexed by tokenizer
	                                                             *    pointer                                       */

	InternetAddressList *rcpt_mime;                         	/**< list of all recipients                         */
	InternetAddressList *rcpt_envelope;                         /**< list of all recipients                         */
	InternetAddressList *from_mime;
	InternetAddressList *from_envelope;

	GList *messages;                                            /**< list of messages that would be reported		*/
	GHashTable *re_cache;                                       /**< cache for matched or not matched regexps		*/
	struct rspamd_config *cfg;                                  /**< pointer to config object						*/
	gchar *last_error;                                          /**< last error										*/
	gint error_code;                                                /**< code of last error								*/
	rspamd_mempool_t *task_pool;                                    /**< memory pool for task							*/
#ifdef HAVE_CLOCK_GETTIME
	struct timespec ts;                                         /**< time of connection								*/
#endif
	struct timeval tv;                                          /**< time of connection								*/
	guint32 scan_milliseconds;                                  /**< how much milliseconds passed					*/
	gboolean pass_all_filters;                                  /**< pass task throught every rule					*/
	gboolean no_log;                                            /**< do not log or write this task to the history	*/
	guint32 parser_recursion;                                   /**< for avoiding recursion stack overflow			*/
	gboolean (*fin_callback)(void *arg);                        /**< calback for filters finalizing					*/
	void *fin_arg;                                              /**< argument for fin callback						*/

	guint32 dns_requests;                                       /**< number of DNS requests per this task			*/

	struct rspamd_dns_resolver *resolver;                       /**< DNS resolver									*/
	struct event_base *ev_base;                                 /**< Event base										*/

	GThreadPool *classify_pool;                                 /**< A pool of classify threads                     */

	struct {
		enum rspamd_metric_action action;                       /**< Action of pre filters							*/
		gchar *str;                                             /**< String describing action						*/
	} pre_result;                                               /**< Result of pre-filters							*/

	ucl_object_t *settings;                                     /**< Settings applied to task						*/
};

/**
 * Construct new task for worker
 */
struct rspamd_task * rspamd_task_new (struct rspamd_worker *worker);
/**
 * Destroy task object and remove its IO dispatcher if it exists
 */
void rspamd_task_free (struct rspamd_task *task, gboolean is_soft);
void rspamd_task_free_hard (gpointer ud);
void rspamd_task_free_soft (gpointer ud);

/**
 * Called if session was restored inside fin callback
 */
void rspamd_task_restore (void *arg);

/**
 * Called if all filters are processed
 * @return TRUE if session should be terminated
 */
gboolean rspamd_task_fin (void *arg);

/**
 * Process task from http message and write reply or call task->fin_handler
 * @param task task to process
 * @param msg incoming http message
 * @param classify_pool classify pool (or NULL)
 * @param process_extra_filters whether to check pre and post filters
 * @return task has been successfully parsed and processed
 */
gboolean rspamd_task_process (struct rspamd_task *task,
	struct rspamd_http_message *msg, GThreadPool *classify_pool,
	gboolean process_extra_filters);

/**
 * Return address of sender or NULL
 * @param task
 * @return
 */
const gchar *rspamd_task_get_sender (struct rspamd_task *task);

#endif /* TASK_H_ */
