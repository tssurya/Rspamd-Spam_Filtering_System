/* Copyright (c) 2010-2011, Vsevolod Stakhov
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
#include "lua_common.h"
#include "upstream.h"
#include "cfg_file.h"


/**
 * This module implements upstreams manipulation from lua
 */
/* Upstream list functions */
LUA_FUNCTION_DEF (upstream_list, create);
LUA_FUNCTION_DEF (upstream_list, destroy);
LUA_FUNCTION_DEF (upstream_list, get_upstream_by_hash);
LUA_FUNCTION_DEF (upstream_list, get_upstream_round_robin);
LUA_FUNCTION_DEF (upstream_list, get_upstream_master_slave);

static const struct luaL_reg upstream_list_m[] = {

	LUA_INTERFACE_DEF (upstream_list, get_upstream_by_hash),
	LUA_INTERFACE_DEF (upstream_list, get_upstream_round_robin),
	LUA_INTERFACE_DEF (upstream_list, get_upstream_master_slave),
	{"__tostring", rspamd_lua_class_tostring},
	{"__gc", lua_upstream_list_destroy},
	{NULL, NULL}
};
static const struct luaL_reg upstream_list_f[] = {
	LUA_INTERFACE_DEF (upstream_list, create),
	{NULL, NULL}
};

/* Upstream functions */
LUA_FUNCTION_DEF (upstream, ok);
LUA_FUNCTION_DEF (upstream, fail);
LUA_FUNCTION_DEF (upstream, get_addr);

static const struct luaL_reg upstream_m[] = {
	LUA_INTERFACE_DEF (upstream, ok),
	LUA_INTERFACE_DEF (upstream, fail),
	LUA_INTERFACE_DEF (upstream, get_addr),
	{"__tostring", rspamd_lua_class_tostring},
	{NULL, NULL}
};

/* Upstream class */

static struct upstream *
lua_check_upstream (lua_State * L)
{
	void *ud = luaL_checkudata (L, 1, "rspamd{upstream}");

	luaL_argcheck (L, ud != NULL, 1, "'upstream' expected");
	return ud ? *((struct upstream **)ud) : NULL;
}

/**
 * Get ip of upstream in numeric form (guint32)
 * @param L
 * @return
 */
static gint
lua_upstream_get_addr (lua_State *L)
{
	struct upstream *up = lua_check_upstream (L);

	if (up) {
		rspamd_lua_ip_push (L, rspamd_upstream_addr (up));
	}
	else {
		lua_pushnil (L);
	}

	return 1;
}

/**
 * Make upstream fail, the second argument is time, if absent the current time is used
 * @param L
 * @return
 */
static gint
lua_upstream_fail (lua_State *L)
{
	struct upstream *up = lua_check_upstream (L);

	if (up) {
		rspamd_upstream_fail (up);
	}

	return 0;
}

/**
 * Make upstream success, the second argument is time, if absent the current time is used
 * @param L
 * @return
 */
static gint
lua_upstream_ok (lua_State *L)
{
	struct upstream *up = lua_check_upstream (L);

	if (up) {
		rspamd_upstream_ok (up);
	}

	return 0;
}

/* Upstream list class */

static struct upstream_list *
lua_check_upstream_list (lua_State * L)
{
	void *ud = luaL_checkudata (L, 1, "rspamd{upstream_list}");

	luaL_argcheck (L, ud != NULL, 1, "'upstream_list' expected");
	return ud ? *((struct upstream_list **)ud) : NULL;
}

/**
 * Create new upstream list from its string definition like '<upstream>,<upstream>;<upstream>'
 * @param L
 * @return upstream list structure
 */
static gint
lua_upstream_list_create (lua_State *L)
{
	struct upstream_list *new = NULL, **pnew;
	const gchar *def;
	char **tokens, **t;
	guint default_port = 0;

	def = luaL_checkstring (L, 1);
	if (def) {
		if (lua_gettop (L) >= 2) {
			default_port = luaL_checkinteger (L, 2);
		}

		tokens = g_strsplit_set (def, ",;", 0);
		if (!tokens || !tokens[0]) {
			goto err;
		}
		t = tokens;

		new = rspamd_upstreams_create ();
		while (*t != NULL) {
			if (!rspamd_upstreams_add_upstream (new, *t, default_port, NULL)) {
				goto err;
			}
			t ++;
		}
		pnew = lua_newuserdata (L, sizeof (struct upstream_list *));
		rspamd_lua_setclass (L, "rspamd{upstream_list}", -1);

		g_strfreev (tokens);
		*pnew = new;
	}

	return 1;
err:
	if (tokens) {
		g_strfreev (tokens);
	}
	if (new) {
		rspamd_upstreams_destroy (new);
	}
	lua_pushnil (L);

	return 1;
}

/**
 * Destroy a single upstream list object
 * @param L
 * @return
 */
static gint
lua_upstream_list_destroy (lua_State *L)
{
	struct upstream_list *upl = lua_check_upstream_list (L);

	rspamd_upstreams_destroy (upl);

	return 0;
}

/**
 * Get upstream by hash from key, params are: key and time (optional)
 * @param L
 * @return
 */
static gint
lua_upstream_list_get_upstream_by_hash (lua_State *L)
{
	struct upstream_list *upl;
	struct upstream *selected, **pselected;
	const gchar *key;
	gsize keyl;

	upl = lua_check_upstream_list (L);
	if (upl) {
		key = luaL_checklstring (L, 2, &keyl);
		if (key) {
			selected = rspamd_upstream_get (upl, RSPAMD_UPSTREAM_HASHED, key,
					(guint)keyl);
			if (selected) {
				pselected = lua_newuserdata (L, sizeof (struct upstream *));
				rspamd_lua_setclass (L, "rspamd{upstream}", -1);
				*pselected = selected;
			}
			else {
				lua_pushnil (L);
			}
		}
		else {
			lua_pushnil (L);
		}
	}
	else {
		lua_pushnil (L);
	}

	return 1;
}

/**
 * Get upstream round robin (by current weight), params are: time (optional)
 * @param L
 * @return
 */
static gint
lua_upstream_list_get_upstream_round_robin (lua_State *L)
{
	struct upstream_list *upl;
	struct upstream *selected, **pselected;

	upl = lua_check_upstream_list (L);
	if (upl) {

		selected = rspamd_upstream_get (upl, RSPAMD_UPSTREAM_ROUND_ROBIN);
		if (selected) {
			pselected = lua_newuserdata (L, sizeof (struct upstream *));
			rspamd_lua_setclass (L, "rspamd{upstream}", -1);
			*pselected = selected;
		}
		else {
			lua_pushnil (L);
		}
	}
	else {
		lua_pushnil (L);
	}

	return 1;
}

/**
 * Get upstream master slave order (by static priority), params are: time (optional)
 * @param L
 * @return
 */
static gint
lua_upstream_list_get_upstream_master_slave (lua_State *L)
{
	struct upstream_list *upl;
	struct upstream *selected, **pselected;

	upl = lua_check_upstream_list (L);
	if (upl) {

		selected = rspamd_upstream_get (upl, RSPAMD_UPSTREAM_MASTER_SLAVE);
		if (selected) {
			pselected = lua_newuserdata (L, sizeof (struct upstream *));
			rspamd_lua_setclass (L, "rspamd{upstream}", -1);
			*pselected = selected;
		}
		else {
			lua_pushnil (L);
		}
	}
	else {
		lua_pushnil (L);
	}

	return 1;
}

static gint
lua_load_upstream_list (lua_State * L)
{
	lua_newtable (L);
	luaL_register (L, NULL, upstream_list_f);

	return 1;
}

void
luaopen_upstream (lua_State * L)
{
	luaL_newmetatable (L, "rspamd{upstream_list}");
	lua_pushstring (L, "__index");
	lua_pushvalue (L, -2);
	lua_settable (L, -3);

	lua_pushstring (L, "class");
	lua_pushstring (L, "rspamd{upstream_list}");
	lua_rawset (L, -3);

	luaL_register (L, NULL,			   upstream_list_m);
	rspamd_lua_add_preload (L, "rspamd_upstream_list", lua_load_upstream_list);

	lua_pop (L, 1);                      /* remove metatable from stack */

	luaL_newmetatable (L, "rspamd{upstream}");
	lua_pushstring (L, "__index");
	lua_pushvalue (L, -2);
	lua_settable (L, -3);

	lua_pushstring (L, "class");
	lua_pushstring (L, "rspamd{upstream}");
	lua_rawset (L, -3);

	luaL_register (L, NULL,		  upstream_m);

	lua_pop (L, 1);                      /* remove metatable from stack */
}
