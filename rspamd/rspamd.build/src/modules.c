/* Dhpr9Fzz */
#include "config.h"
extern module_t surbl_module;
extern module_t regexp_module;
extern module_t chartable_module;
extern module_t fuzzy_check_module;
extern module_t spf_module;
extern module_t dkim_module;


module_t *modules[] = {
&surbl_module,
&regexp_module,
&chartable_module,
&fuzzy_check_module,
&spf_module,
&dkim_module,
NULL
};
extern worker_t normal_worker;
extern worker_t controller_worker;
extern worker_t smtp_proxy_worker;
extern worker_t fuzzy_worker;
extern worker_t lua_worker;


worker_t *workers[] = {
&normal_worker,
&controller_worker,
&smtp_proxy_worker,
&fuzzy_worker,
&lua_worker,
NULL
};
