#ifndef RSPAMD_CONFIG_H_IN
#define RSPAMD_CONFIG_H_IN

#define HAVE_SYS_TYPES_H    1

#define HAVE_SYS_TIME_H     1

#define HAVE_SYS_UIO_H      1

#define HAVE_FCNTL_H        1

#define HAVE_MATH_H         1

#define HAVE_STDIO_H        1

#define HAVE_STDLIB_H       1

#define HAVE_STRING_H       1

#define HAVE_STRINGS_H      1

#define HAVE_TIME_H         1

#define HAVE_UNISTD_H       1

#define HAVE_SYSEXITS_H     1

#define HAVE_STDINT_H       1

#define HAVE_INTTYPES_H     1

/* #undef HAVE_STDBOOL_H */

#define HAVE_ARPA_INET_H    1
#define HAVE_NETINET_IN_H   1

#define HAVE_NETDB_H        1

#define HAVE_POLL_H         1

#define HAVE_SYSLOG_H       1

/* #undef HAVE_SIGINFO_H */

/* #undef HAVE_LIBUTIL_H */

#define HAVE_LIBGEN_H       1

#define HAVE_SEARCH_H       1

#define HAVE_LOCALE_H       1

#define HAVE_GRP_H          1
#define HAVE_PWD_H          1

#define HAVE_GLOB_H         1

#define HAVE_ENDIAN_H       1
/* #undef HAVE_SYS_ENDIAN_H */
/* #undef HAVE_MACHINE_ENDIAN_H */
#define HAVE_SYS_CDEFS_H    1

#define LIBEVENT_EVHTTP     1

 #ifdef HAVE_ENDIAN_H
 #include <endian.h>
 #else 
 #ifdef HAVE_SYS_ENDIAN_H
 #include <sys/endian.h>
 #else
 #ifdef HAVE_MACHINE_ENDIAN_H
 #include <machine/endian.h>
 #else
 #define LITTLE_ENDIAN   1234    
 #define BIG_ENDIAN      4321    
 #if defined(i386) || defined(__i386) || defined(__amd64) || \
 	 defined(BIT_ZERO_ON_RIGHT) || defined(__x86__)
 #define BYTE_ORDER      LITTLE_ENDIAN
 #endif
 
 #if defined(__sparc) || defined(__sparc__) || defined(__powerpc__)
 #define BYTE_ORDER      BIG_ENDIAN
 #endif
 
 #endif /* HAVE_MACHINE_ENDIAN_H */
 #endif /* HAVE_SYS_ENDIAN_H */
 #endif /* HAVE_ENDIAN_H */
 
 #if !defined(BYTE_ORDER)
 # error "Undefined or unknown BYTE_ORDER"
 #endif
 
#ifdef _MSC_VER
# define RSPAMD_PACKED(name) \
    __pragma(pack(push, 1)) struct name __pragma(pack(pop))
#elif defined(__GNUC__)
# define RSPAMD_PACKED(name) struct __attribute__((packed)) name
#else
# define RSPAMD_PACKED(name) struct name
#endif


#define HAVE_SYS_QUEUE_H    1
#define HAVE_SYS_MMAN_H     1
#define HAVE_SYS_SOCKET_H   1
#define HAVE_SYS_UN_H       1
#define HAVE_SYS_STAT_H     1
#define HAVE_SYS_WAIT_H     1
#define HAVE_SYS_PARAM_H    1
#define HAVE_SYS_FILE_H     1
#define HAVE_SYS_RESOURCE_H 1
/* #undef HAVE_SYS_TIMEB_H */
#define HAVE_SYS_UTSNAME_H  1

/* #undef HAVE_PIDFILE */

/* #undef HAVE_PIDFILE_FILENO */

/* #undef HAVE_SETPROCTITLE */

#define HAVE_GETPAGESIZE    1

#define HAVE_NANOSLEEP      1

#define HAVE_PATH_MAX       1

#define HAVE_MAXPATHLEN     1

#define HAVE_MMAP_SHARED    1

#define HAVE_MMAP_ANON      1

/* #undef HAVE_MMAP_NOCORE */

#define HAVE_O_DIRECT       1

#define HAVE_FADVISE        1

#define HAVE_FALLOCATE      1
#define HAVE_POSIX_FALLOCATE 1

#define HAVE_FDATASYNC      1
/* #undef HAVE_COMPATIBLE_QUEUE_H */

#define HAVE_SC_NPROCESSORS_ONLN 1

#define HAVE_VFORK          1

#define HAVE_WAIT4          1

#define HAVE_WAITPID        1

#define HAVE_FLOCK          1

#define HAVE_TANHL          1
#define HAVE_TANH           1

#define HAVE_EXPL           1
#define HAVE_EXP2L          1

#define HAVE_SA_SIGINFO     1

#define DEBUG_MODE          1

#define GMIME24             1

/* #undef HAVE_CLOCK_VIRTUAL */
#define HAVE_CLOCK_PROCESS_CPUTIME_ID  1

#define HAVE_SETITIMER      1

#define HAVE_INET_PTON      1

#define      WITHOUT_PERL        1

#define WITH_LUA            1

/* #undef WITH_PROFILER */

/* #undef WITH_JUDY */

/* #undef WITH_DB */

#define WITH_SQLITE         1

/* #undef WITH_GPERF_TOOLS */

/* #undef WITH_SYSTEM_HIREDIS */

#define HAVE_ASM_PAUSE      1

/* #undef BUILD_STATIC */

#define HAVE_SENDFILE       1
#define HAVE_SYS_SENDFILE_H 1
#define HAVE_SYS_EVENTFD_H  1
#define HAVE_AIO_H          1
/* #undef HAVE_LIBAIO_H */

#define HAVE_MKSTEMP        1

#define HAVE_CLOCK_GETTIME  1

#define HAVE_OPENSSL		 1

/* #undef GLIB_COMPAT */
/* #undef GLIB_RE_COMPAT */
/* #undef GLIB_UNISCRIPT_COMPAT */
/* #undef GLIB_HASH_COMPAT */
#define PARAM_H_HAS_BITSET  1

/* #undef HAVE_FETCH_H */
/* #undef CURL_FOUND */

/* #undef HAVE_READPASSPHRASE_H */
#define HAVE_TERMIOS_H      1

#define HAVE_IPV6_V6ONLY    1

#define HAVE_CTYPE_H        1
/* #undef HAVE_SCHED_YEILD */
#define HAVE_PTHREAD_PROCESS_SHARED 1

/* Configure allocator */
#define uthash_malloc(sz) g_slice_alloc(sz)
#define uthash_free(ptr,sz) g_slice_free1(sz, ptr)

#define RSPAMD_CONFDIR "/etc/rspamd"
#define RSPAMD_RUNDIR "/run/rspamd"
#define RSPAMD_LOGDIR "/var/log/rspamd"
#define RSPAMD_DBDIR "/var/lib/rspamd"
#define RSPAMD_EXAMPLESDIR "/usr/local/share/examples/rspamd"
#define RSPAMD_PLUGINSDIR "/usr/local/share/rspamd"
#define RSPAMD_WWWDIR "/usr/local/share/rspamd/www"
#define RSPAMD_PREFIX "/usr/local"

#define RVERSION          "0.8.1"
#define RID               ""
#define RSPAMD_MASTER_SITE_URL "https://rspamd.com"

#define MODULES_NUM        

/* sys/types */
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

/* cdefs */
#ifdef HAVE_SYS_CDEFS_H
#include <sys/cdefs.h>
#endif

/* sys/param */
#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

/* stdint */
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif defined(HAVE_INTTYPES_H)
#include <inttypes.h>
#endif

/* stdbool */
#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif

/* queue */
#ifdef HAVE_COMPATIBLE_QUEUE_H
#ifdef HAVE_SYS_QUEUE_H
#include <sys/queue.h>
#endif
#else
#include "queue.h"
#endif

/* sys/socket */
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

/* sys/file.h */
#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif

/* sys/uio.h */
#ifdef HAVE_SYS_UIO_H
#include <sys/uio.h>
#endif

/* sys/mman */
#ifdef HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif

/* netinet issues */
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

/* unix sockets */
#ifdef HAVE_SYS_UN_H
#include <sys/un.h>
#endif

/* netdb */
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

/* stdlib */
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

/* stdio */
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

/* time */
#ifdef HAVE_TIME_H
#include <time.h>
#endif

/* timedb */
#ifdef HAVE_SYS_TIMEB_H
#include <sys/timeb.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

/* sysexits */
#ifdef HAVE_SYSEXITS_H
#include <sysexits.h>
#endif

/* unistd */
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif


/* string */
#ifdef HAVE_STRING_H
#include <string.h>
#endif

/* strings */
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

/* fcntl */
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

/* Linux specific falloc.h */
#ifdef HAVE_LINUX_FALLOC_H
#include <linux/falloc.h>
#endif

/* poll */
#ifdef HAVE_POLL_H
#include <poll.h>
#endif

/* sys/stat */
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

/* sys/wait */
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

/* sys/resource.h */
#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif

/* math */
#ifdef HAVE_MATH_H
#include <math.h>
#endif

/* libutil */
#ifdef HAVE_LIBUTIL_H
#include <libutil.h>
#endif

/* pwd and grp */
#ifdef HAVE_PWD_H
#include <pwd.h>
#endif

#ifdef HAVE_GRP_H
#include <grp.h>
#endif

#ifdef HAVE_GLOB_H
#include <glob.h>
#endif

/* syslog */
#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif

#ifdef HAVE_LIBGEN_H
#include <libgen.h>
#define HAVE_DIRNAME 1
#endif

#ifdef HAVE_SEARCH_H
#include <search.h>
#endif

#ifdef WITH_GPERF_TOOLS
#include <google/profiler.h>
#endif

#ifdef HAVE_SCHED_YIELD
#include <sched.h>
#endif

#include <errno.h>
#include <signal.h>
#ifdef HAVE_SIGINFO_H
# include <siginfo.h>
#endif
#include <event.h>
#if _EVENT_NUMERIC_VERSION > 0x02000000
# include <event2/thread.h>
#endif
#include <glib.h>


#ifdef GLIB_RE_COMPAT
#include "gregex.h"
#endif
#ifdef GLIB_UNISCRIPT_COMPAT
#include "guniscript.h"
#endif
#ifdef GLIB_COMPAT
#include "gchecksum.h"
#endif

#if (GLIB_MAJOR_VERSION == 2) && (GLIB_MINOR_VERSION < 14)
typedef off_t goffset;
#endif

#ifndef BUILD_STATIC
#include <gmodule.h>
#endif

#ifndef NO_GMIME
#include <gmime/gmime.h>
#endif

/* Forwarded declaration */
struct module_ctx;
struct rspamd_config;
struct rspamd_worker;

typedef struct module_s {
	const gchar *name;
	int (*module_init_func)(struct rspamd_config *cfg, struct module_ctx **ctx);
	int (*module_config_func)(struct rspamd_config *cfg);
	int (*module_reconfig_func)(struct rspamd_config *cfg);
	int (*module_attach_controller_func)(struct module_ctx *ctx, 
		GHashTable *custom_commands);
} module_t;

typedef struct worker_s {
	const gchar *name;
	gpointer (*worker_init_func)(struct rspamd_config *cfg);
	void (*worker_start_func)(struct rspamd_worker *worker);
	gboolean has_socket;
	gboolean unique;
	gboolean threaded;
	gboolean killable;
	gint listen_type;
} worker_t;

extern module_t *modules[];
extern worker_t *workers[];

#endif
