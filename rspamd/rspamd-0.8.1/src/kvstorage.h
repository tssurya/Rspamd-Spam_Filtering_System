/* Copyright (c) 2011, Vsevolod Stakhov
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

#ifndef KVSTORAGE_H_
#define KVSTORAGE_H_

#include "config.h"

struct rspamd_kv_cache;
struct rspamd_kv_backend;
struct rspamd_kv_storage;
struct rspamd_kv_expire;
struct rspamd_kv_element;

/* Locking definitions */
#if ((GLIB_MAJOR_VERSION == 2) && (GLIB_MINOR_VERSION > 30))
#define RW_R_LOCK g_rw_lock_reader_lock
#define RW_R_UNLOCK g_rw_lock_reader_unlock
#define RW_W_LOCK g_rw_lock_writer_lock
#define RW_W_UNLOCK g_rw_lock_writer_unlock
#else
#define RW_R_LOCK g_static_rw_lock_reader_lock
#define RW_R_UNLOCK g_static_rw_lock_reader_unlock
#define RW_W_LOCK g_static_rw_lock_writer_lock
#define RW_W_UNLOCK g_static_rw_lock_writer_unlock
#endif

/* Callbacks for cache */
typedef void (*cache_init)(struct rspamd_kv_cache *cache);
typedef struct rspamd_kv_element * (*cache_insert)(struct rspamd_kv_cache *cache,
	gpointer key, guint keylen, gpointer value, gsize len);
typedef gboolean (*cache_replace)(struct rspamd_kv_cache *cache, gpointer key,
	guint keylen,
	struct rspamd_kv_element *elt);
typedef struct rspamd_kv_element * (*cache_lookup)(struct rspamd_kv_cache *cache,
	gpointer key, guint keylen);
typedef struct rspamd_kv_element * (*cache_delete)(struct rspamd_kv_cache *cache,
	gpointer key, guint keylen);
typedef void (*cache_steal)(struct rspamd_kv_cache *cache,
	struct rspamd_kv_element * elt);
typedef void (*cache_destroy)(struct rspamd_kv_cache *cache);

/* Callbacks for backend */
typedef void (*backend_init)(struct rspamd_kv_backend *backend);
typedef gboolean (*backend_insert)(struct rspamd_kv_backend *backend,
	gpointer key, guint keylen,
	struct rspamd_kv_element *elt);
typedef gboolean (*backend_replace)(struct rspamd_kv_backend *backend,
	gpointer key, guint keylen,
	struct rspamd_kv_element *elt);
typedef struct rspamd_kv_element * (*backend_lookup)(struct rspamd_kv_backend *
	backend, gpointer key,
	guint keylen);
typedef void (*backend_delete)(struct rspamd_kv_backend *backend, gpointer key,
	guint keylen);
typedef gboolean (*backend_sync)(struct rspamd_kv_backend *backend);
typedef gboolean (*backend_incref)(struct rspamd_kv_backend *backend,
	gpointer key, guint keylen);
typedef void (*backend_destroy)(struct rspamd_kv_backend *backend);

/* Callbacks for expire */
typedef void (*expire_init)(struct rspamd_kv_expire *expire);
typedef void (*expire_insert)(struct rspamd_kv_expire *expire,
	struct rspamd_kv_element *elt);
typedef void (*expire_delete)(struct rspamd_kv_expire *expire,
	struct rspamd_kv_element *elt);
typedef gboolean (*expire_step)(struct rspamd_kv_expire *expire,
	struct rspamd_kv_storage *storage,
	time_t now, gboolean forced);
typedef void (*expire_destroy)(struct rspamd_kv_expire *expire);


/* Flags of element */
enum rspamd_kv_flags {
	KV_ELT_ARRAY = 1 << 0,
	KV_ELT_PERSISTENT = 1 << 1,
	KV_ELT_DIRTY = 1 << 2,
	KV_ELT_OUSTED = 1 << 3,
	KV_ELT_NEED_FREE = 1 << 4,
	KV_ELT_INTEGER = 1 << 5,
	KV_ELT_NEED_INSERT = 1 << 6,
	KV_ELT_NEED_EXPIRE = 1 << 7
};

#define ELT_DATA(elt) (gchar *)(elt)->data + (elt)->keylen + 1
#define ELT_LONG(elt) *((glong *)((elt)->data + (elt)->keylen + 1))
#define ELT_KEY(elt) (gchar *)(elt)->data
#define ELT_SIZE(elt) elt->size + sizeof(struct rspamd_kv_element) + \
	elt->keylen + 1

/* Common structures description */

struct rspamd_kv_element {
	time_t age;                                 /*< age of element */
	guint32 expire;                             /*< expire of element */
	gint flags;                                 /*< element flags  */
	gsize size;                                 /*< size of element */
	TAILQ_ENTRY (rspamd_kv_element) entry;      /*< list entry */
	guint keylen;                               /*< length of key */

	gpointer p;                                 /*< pointer to data */
	gchar data[1];                              /*< expandable data */
};

struct rspamd_kv_cache {
	cache_init init_func;                       /*< this callback is called on kv storage initialization */
	cache_insert insert_func;                   /*< this callback is called when element is inserted */
	cache_replace replace_func;                 /*< this callback is called when element is replace */
	cache_lookup lookup_func;                   /*< this callback is used for lookup of element */
	cache_delete delete_func;                   /*< this callback is called when an element is deleted */
	cache_steal steal_func;                     /*< this callback is used to replace duplicates in cache */
	cache_destroy destroy_func;                 /*< this callback is used for destroying all elements inside cache */
};
struct rspamd_kv_backend {
	backend_init init_func;                     /*< this callback is called on kv storage initialization */
	backend_insert insert_func;                 /*< this callback is called when element is inserted */
	backend_replace replace_func;               /*< this callback is called when element is replaced */
	backend_lookup lookup_func;                 /*< this callback is used for lookup of element */
	backend_delete delete_func;                 /*< this callback is called when an element is deleted */
	backend_sync sync_func;                     /*< this callback is called when backend need to be synced */
	backend_incref incref_func;                 /*< this callback is called when element must be ref'd */
	backend_destroy destroy_func;               /*< this callback is used for destroying all elements inside backend */
};
struct rspamd_kv_expire {
	expire_init init_func;                      /*< this callback is called on kv storage initialization */
	expire_insert insert_func;                  /*< this callback is called when element is inserted */
	expire_step step_func;                      /*< this callback is used when cache is full */
	expire_delete delete_func;                  /*< this callback is called when an element is deleted */
	expire_destroy destroy_func;                /*< this callback is used for destroying all elements inside expire */
};

/* Main kv storage structure */

struct rspamd_kv_storage {
	struct rspamd_kv_cache *cache;
	struct rspamd_kv_backend *backend;
	struct rspamd_kv_expire *expire;

	gsize elts;                                 /*< current elements count in a storage */
	gsize max_elts;                             /*< maximum number of elements in a storage */

	gsize memory;                               /*< memory eaten */
	gsize max_memory;                           /*< memory limit */

	gint id;                                    /* char ID */
	gchar *name;                                /* numeric ID */

	gboolean no_overwrite;                      /* do not overwrite data with the same keys */
#if ((GLIB_MAJOR_VERSION == 2) && (GLIB_MINOR_VERSION > 30))
	GRWLock rwlock;                             /* rwlock in new glib */
#else
	GStaticRWLock rwlock;                       /* rwlock for threaded access */
#endif
};

/** Create new kv storage */
struct rspamd_kv_storage * rspamd_kv_storage_new (gint id, const gchar *name,
	struct rspamd_kv_cache *cache, struct rspamd_kv_backend *backend,
	struct rspamd_kv_expire *expire,
	gsize max_elts, gsize max_memory, gboolean no_overwrite);

/** Insert new element to the kv storage */
gboolean rspamd_kv_storage_insert (struct rspamd_kv_storage *storage,
	gpointer key,
	guint keylen,
	gpointer data,
	gsize len,
	gint flags,
	guint expire);

/** Insert element only in cache */
gboolean rspamd_kv_storage_insert_cache (struct rspamd_kv_storage *storage,
	gpointer key,
	guint keylen,
	gpointer data,
	gsize len,
	gint flags,
	guint expire,
	struct rspamd_kv_element **pelt);

/** Replace an element in the kv storage */
gboolean rspamd_kv_storage_replace (struct rspamd_kv_storage *storage,
	gpointer key,
	guint keylen,
	struct rspamd_kv_element *elt);

/** Increment value in kvstorage */
gboolean rspamd_kv_storage_increment (struct rspamd_kv_storage *storage,
	gpointer key,
	guint keylen,
	glong *value);

/** Lookup an element inside kv storage */
struct rspamd_kv_element * rspamd_kv_storage_lookup (
	struct rspamd_kv_storage *storage,
	gpointer key,
	guint keylen,
	time_t now);

/** Expire an element from kv storage */
struct rspamd_kv_element * rspamd_kv_storage_delete (
	struct rspamd_kv_storage *storage,
	gpointer key,
	guint keylen);

/** Destroy kv storage */
void rspamd_kv_storage_destroy (struct rspamd_kv_storage *storage);

/** Insert array */
gboolean rspamd_kv_storage_insert_array (struct rspamd_kv_storage *storage,
	gpointer key,
	guint keylen,
	guint elt_size,
	gpointer data,
	gsize len,
	gint flags,
	guint expire);

/** Set element inside array */
gboolean rspamd_kv_storage_set_array (struct rspamd_kv_storage *storage,
	gpointer key,
	guint keylen,
	guint elt_num,
	gpointer data,
	gsize len,
	time_t now);

/** Get element inside array */
gboolean rspamd_kv_storage_get_array (struct rspamd_kv_storage *storage,
	gpointer key,
	guint keylen,
	guint elt_num,
	gpointer *data,
	gsize *len,
	time_t now);

/* Hash table functions */
guint kv_elt_hash_func (gconstpointer e);
gboolean kv_elt_compare_func (gconstpointer e1, gconstpointer e2);

/**
 * LRU expire
 */
struct rspamd_kv_expire * rspamd_lru_expire_new (void);

/**
 * Ordinary hash
 */
struct rspamd_kv_cache * rspamd_kv_hash_new (void);

/**
 * Radix tree
 */
struct rspamd_kv_cache * rspamd_kv_radix_new (void);

#ifdef WITH_JUDY
/**
 * Judy tree
 */
struct rspamd_kv_cache * rspamd_kv_judy_new (void);
#endif

#endif /* KVSTORAGE_H_ */
