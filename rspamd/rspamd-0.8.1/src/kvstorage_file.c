/* Copyright (c) 2010, Vsevolod Stakhov
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
#include "kvstorage.h"
#include "kvstorage_file.h"
#include "util.h"
#include "main.h"

struct file_op {
	struct rspamd_kv_element *elt;
	enum {
		FILE_OP_INSERT,
		FILE_OP_DELETE,
		FILE_OP_REPLACE
	} op;
	guint32 ref;
};

/* Main file structure */
struct rspamd_file_backend {
	backend_init init_func;                     /*< this callback is called on kv storage initialization */
	backend_insert insert_func;                 /*< this callback is called when element is inserted */
	backend_replace replace_func;               /*< this callback is called when element is replaced */
	backend_lookup lookup_func;                 /*< this callback is used for lookup of element */
	backend_delete delete_func;                 /*< this callback is called when an element is deleted */
	backend_sync sync_func;                     /*< this callback is called when backend need to be synced */
	backend_incref incref_func;                 /*< this callback is called when element must be ref'd */
	backend_destroy destroy_func;               /*< this callback is used for destroying all elements inside backend */
	gchar *filename;
	gchar *dirname;
	guint dirlen;
	guint sync_ops;
	guint levels;
	GQueue *ops_queue;
	GHashTable *ops_hash;
	gboolean do_fsync;
	gboolean do_ref;
	gboolean initialized;
};

static const gchar hexdigits[] =
{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e',
	 'f'};

/* Generate file name for operation */
static gboolean
get_file_name (struct rspamd_file_backend *db,
	gchar *key,
	guint keylen,
	gchar *filebuf,
	guint buflen)
{
	gchar *p = filebuf, *end = filebuf + buflen,
	*k = key, t;
	guint i;

	/* First copy backend dirname to file buf */
	if (buflen <= db->dirlen) {
		return FALSE;
	}
	memcpy (p, db->dirname, db->dirlen);
	p += db->dirlen;
	*p++ = G_DIR_SEPARATOR;
	for (i = 0; i < MIN (keylen, db->levels); i++) {
		if (p == end) {
			/* Filebuf is not large enough */
			return FALSE;
		}
		t = *k;
		*p++ = hexdigits[(t & 0xf) ^ ((t & 0xf0) >> 4)];
		*p++ = G_DIR_SEPARATOR;
		k++;
	}
	/* Now we have directory, append base64 encoded filename */
	k = key;
	if (end - p < (gint)keylen * 2 + 1) {
		/* Filebuf is not large enough */
		return FALSE;
	}

	i = 0;
	while (k < key + keylen) {
		t = *k;
		*p++ = hexdigits[(t >> 4) & 0xf];
		*p++ = hexdigits[t & 0xf];
		k++;
	}
	*p = '\0';

	return TRUE;
}

/* Read reference from specified file */
static guint32
file_get_ref (gint fd)
{
	guint32 target;

	if (read (fd, &target, sizeof (guint32)) != sizeof (guint32)) {
		return 0;
	}

	return target;
}

/* Set reference to specified file */
static gboolean
file_set_ref (gint fd, guint32 ref)
{
	if (write (fd, &ref, sizeof (guint32)) != sizeof (guint32)) {
		return FALSE;
	}

	return TRUE;
}

/*
 * Open file, set posix_fadvise and all necessary flags
 */
static gint
file_open_fd (const gchar *path, gsize *len, gint flags)
{
	gint fd;
	struct stat st;

	if ((flags & O_CREAT) != 0) {
		/* Open file */
		if ((fd = open (path, flags, S_IRUSR | S_IWUSR | S_IRGRP)) != -1) {
			rspamd_fallocate (fd, 0, *len);
#ifdef HAVE_FADVISE
			posix_fadvise (fd, 0, *len, POSIX_FADV_SEQUENTIAL);
#endif
		}
	}
	else {
		/* Open file */
		if ((fd = open (path, flags)) == -1) {
			return -1;
		}

		if (fstat (fd, &st) == -1) {
			close (fd);
			return -1;
		}

#ifdef HAVE_FADVISE
		posix_fadvise (fd, 0, st.st_size, POSIX_FADV_SEQUENTIAL);
#endif
		*len = st.st_size;
	}

	return fd;
}

/* Process single file operation */
static gboolean
file_process_single_op (struct rspamd_file_backend *db,
	struct file_op *op,
	gint *pfd)
{
	gchar filebuf[PATH_MAX];
	gint fd;
	gsize len;
	struct iovec iov[2];
	guint32 ref;

	/* Get filename */
	if (!get_file_name (db, ELT_KEY (op->elt), op->elt->keylen, filebuf,
		sizeof (filebuf))) {
		return FALSE;
	}

	if (db->do_ref) {
		len = ELT_SIZE (op->elt) + sizeof (guint32);
	}
	else {
		len = ELT_SIZE (op->elt);
	}

	if (op->op == FILE_OP_DELETE) {
		if (db->do_ref) {
			if ((fd = file_open_fd (filebuf, &len, O_RDWR)) == -1) {
				*pfd = -1;
				return FALSE;
			}
			if ((ref = file_get_ref (fd)) <= 1) {
				/* Refcount is not enough, remove file */
				close (fd);
				*pfd = -1;
				return unlink (filebuf) != -1;
			}
			else {
				/* Decrease ref */
				lseek (fd, 0, SEEK_SET);
				if (!file_set_ref (fd, --ref)) {
					*pfd = fd;
					return FALSE;
				}
			}
		}
		else {
			*pfd = -1;
			return unlink (filebuf) != -1;
		}
	}
	else {
		if ((fd =
			file_open_fd (filebuf, &len, O_CREAT | O_WRONLY | O_TRUNC)) == -1) {
			*pfd = -1;
			return FALSE;
		}
		if (db->do_ref) {
			iov[0].iov_base = &op->ref;
			iov[0].iov_len = sizeof (guint32);
			iov[1].iov_base = op->elt;
			iov[1].iov_len = ELT_SIZE (op->elt);
			if (writev (fd, iov, G_N_ELEMENTS (iov)) == -1) {
				msg_info ("%d: %s", errno, strerror (errno));
				*pfd = fd;
				return FALSE;
			}
		}
		else {
			if (write (fd, op->elt, ELT_SIZE (op->elt)) == -1) {
				msg_info ("%d: %s", errno, strerror (errno));
				*pfd = fd;
				return FALSE;
			}
		}
	}

	*pfd = fd;
	return TRUE;
}

/* Sync vector of descriptors */
static void
file_sync_fds (gint *fds, gint len, gboolean do_fsync)
{
	gint i, fd;

	for (i = 0; i < len; i++) {
		fd = fds[i];
		if (fd != -1) {
			if (do_fsync) {
#ifdef HAVE_FDATASYNC
				fdatasync (fd);
#else
				fsync (fd);
#endif
			}
			close (fd);
		}
	}
}

/* Process operations queue */
static gboolean
file_process_queue (struct rspamd_kv_backend *backend)
{
	struct rspamd_file_backend *db = (struct rspamd_file_backend *)backend;
	struct file_op *op;
	GList *cur;
	gint *fds, i = 0, len;

	len = g_queue_get_length (db->ops_queue);
	if (len == 0) {
		/* Nothing to process */
		return TRUE;
	}

	fds = g_slice_alloc (len * sizeof (gint));
	cur = db->ops_queue->head;
	while (cur) {
		op = cur->data;
		if (!file_process_single_op (db, op, &fds[i])) {
			file_sync_fds (fds, i, db->do_fsync);
			g_slice_free1 (len * sizeof (gint), fds);
			return FALSE;
		}
		i++;
		cur = g_list_next (cur);
	}

	file_sync_fds (fds, i, db->do_fsync);
	g_slice_free1 (len * sizeof (gint), fds);

	/* Clean the queue */
	g_hash_table_remove_all (db->ops_hash);
	cur = db->ops_queue->head;
	while (cur) {
		op = cur->data;
		if (op->op == FILE_OP_DELETE ||
			((op->elt->flags & KV_ELT_NEED_FREE) != 0 &&
			(op->elt->flags & KV_ELT_NEED_INSERT) == 0)) {
			/* Also clean memory */
			g_slice_free1 (ELT_SIZE (op->elt), op->elt);
		}
		else {
			/* Unset dirty flag */
			op->elt->flags &= ~KV_ELT_DIRTY;
		}
		g_slice_free1 (sizeof (struct file_op), op);
		cur = g_list_next (cur);
	}

	g_queue_clear (db->ops_queue);

	return TRUE;

}


/* Make 16 directories for each level */
static gboolean
rspamd_recursive_mkdir (guint levels)
{
	guint i;
	gchar nbuf[5];

	/* Create directories for backend */
	if (levels > 0) {
		/* Create 16 directories */
		for (i = 0; i < 16; i++) {
			rspamd_snprintf (nbuf, sizeof (nbuf), "./%c", hexdigits[i]);
			if (mkdir (nbuf, 0755) != 0 && errno != EEXIST) {
				msg_info ("cannot create directory %s: %s", nbuf,
					strerror (errno));
				return FALSE;
			}
			else if (levels > 1) {
				if (chdir (nbuf) == -1) {
					msg_err ("chdir to %s failed: %s", nbuf, strerror (errno));
					return FALSE;
				}
				if (!rspamd_recursive_mkdir (levels - 1)) {
					return FALSE;
				}
				if (chdir ("../") == -1) {
					msg_err ("chdir to ../ failed: %s", strerror (errno));
					return FALSE;
				}
			}
		}
	}
	return TRUE;

}

/* Backend callbacks */
static void
rspamd_file_init (struct rspamd_kv_backend *backend)
{
	struct rspamd_file_backend *db = (struct rspamd_file_backend *)backend;
	gchar pathbuf[PATH_MAX];

	/* Save current directory */
	if (getcwd (pathbuf, sizeof (pathbuf) - 1) == NULL) {
		pathbuf[0] = '\0';
		msg_err ("getcwd failed: %s", strerror (errno));
		goto err;
	}

	/* Chdir to the working dir */
	if (chdir (db->dirname) == -1) {
		msg_err ("chdir failed: %s", strerror (errno));
		goto err;
	}

	/* Create directories for backend */
	if (!rspamd_recursive_mkdir (db->levels)) {
		goto err;
	}

	db->initialized = TRUE;

	if (chdir (pathbuf) == -1) {
		msg_err ("chdir to %s failed: %s", pathbuf, strerror (errno));
	}
	return;
err:
	if (pathbuf[0] != '\0') {
		if (chdir (pathbuf) == -1) {
			msg_err ("chdir to %s failed: %s", pathbuf, strerror (errno));
		}
	}
}

static gboolean
rspamd_file_insert (struct rspamd_kv_backend *backend,
	gpointer key,
	guint keylen,
	struct rspamd_kv_element *elt)
{
	struct rspamd_file_backend *db = (struct rspamd_file_backend *)backend;
	struct file_op *op;
	struct rspamd_kv_element search_elt;

	search_elt.keylen = keylen;
	search_elt.p = key;

	if (!db->initialized) {
		return FALSE;
	}

	if ((op = g_hash_table_lookup (db->ops_hash, &search_elt)) != NULL) {
		/* We found another op with such key in this queue */
		if (op->op == FILE_OP_DELETE || (op->elt->flags & KV_ELT_NEED_FREE) !=
			0) {
			/* Also clean memory */
			g_hash_table_steal (db->ops_hash, &search_elt);
			g_slice_free1 (ELT_SIZE (op->elt), op->elt);
		}
		op->op = FILE_OP_INSERT;
		op->ref++;
		op->elt = elt;
		elt->flags |= KV_ELT_DIRTY;
		g_hash_table_insert (db->ops_hash, elt, op);
	}
	else {
		op = g_slice_alloc (sizeof (struct file_op));
		op->op = FILE_OP_INSERT;
		op->elt = elt;
		op->ref = 1;
		elt->flags |= KV_ELT_DIRTY;

		g_queue_push_head (db->ops_queue, op);
		g_hash_table_insert (db->ops_hash, elt, op);
	}

	if (db->sync_ops > 0 && g_queue_get_length (db->ops_queue) >=
		db->sync_ops) {
		return file_process_queue (backend);
	}

	return TRUE;
}

static gboolean
rspamd_file_replace (struct rspamd_kv_backend *backend,
	gpointer key,
	guint keylen,
	struct rspamd_kv_element *elt)
{
	struct rspamd_file_backend *db = (struct rspamd_file_backend *)backend;
	struct file_op *op;
	struct rspamd_kv_element search_elt;

	search_elt.keylen = keylen;
	search_elt.p = key;

	if (!db->initialized) {
		return FALSE;
	}
	if ((op = g_hash_table_lookup (db->ops_hash, &search_elt)) != NULL) {
		/* We found another op with such key in this queue */
		if (op->op == FILE_OP_DELETE || (op->elt->flags & KV_ELT_NEED_FREE) !=
			0) {
			/* Also clean memory */
			g_hash_table_steal (db->ops_hash, &search_elt);
			g_slice_free1 (ELT_SIZE (op->elt), op->elt);
		}
		op->op = FILE_OP_REPLACE;
		op->elt = elt;
		elt->flags |= KV_ELT_DIRTY;
		g_hash_table_insert (db->ops_hash, elt, op);
	}
	else {
		op = g_slice_alloc (sizeof (struct file_op));
		op->op = FILE_OP_REPLACE;
		op->elt = elt;
		op->ref = 1;
		elt->flags |= KV_ELT_DIRTY;

		g_queue_push_head (db->ops_queue, op);
		g_hash_table_insert (db->ops_hash, elt, op);
	}

	if (db->sync_ops > 0 && g_queue_get_length (db->ops_queue) >=
		db->sync_ops) {
		return file_process_queue (backend);
	}

	return TRUE;
}

static struct rspamd_kv_element *
rspamd_file_lookup (struct rspamd_kv_backend *backend,
	gpointer key,
	guint keylen)
{
	struct rspamd_file_backend *db = (struct rspamd_file_backend *)backend;
	struct file_op *op;
	struct rspamd_kv_element *elt = NULL;
	gchar filebuf[PATH_MAX];
	gint fd;
	struct rspamd_kv_element search_elt;
	gsize len;

	search_elt.keylen = keylen;
	search_elt.p = key;

	if (!db->initialized) {
		return NULL;
	}
	/* First search in ops queue */
	if ((op = g_hash_table_lookup (db->ops_hash, &search_elt)) != NULL) {
		if (op->op == FILE_OP_DELETE) {
			/* To delete, so assume it as not found */
			return NULL;
		}
		return op->elt;
	}

	/* Get filename */
	if (!get_file_name (db, key, keylen, filebuf, sizeof (filebuf))) {
		return NULL;
	}

	if ((fd = file_open_fd (filebuf, &len, O_RDONLY)) == -1) {
		return NULL;
	}

	/* Read element */
	if (db->do_ref) {
		lseek (fd, sizeof (guint32), SEEK_CUR);
		len -= sizeof (guint32);
	}
	elt = g_malloc (len);
	if (read (fd, elt, len) == -1) {
		g_free (elt);
		close (fd);
		return NULL;
	}

	close (fd);

	elt->flags &= ~(KV_ELT_DIRTY | KV_ELT_NEED_FREE);

	return elt;
}

static void
rspamd_file_delete (struct rspamd_kv_backend *backend,
	gpointer key,
	guint keylen)
{
	struct rspamd_file_backend *db = (struct rspamd_file_backend *)backend;
	gchar filebuf[PATH_MAX];
	struct rspamd_kv_element search_elt;
	struct file_op *op;
	gsize len;
	gint fd;
	guint32 ref;

	if (!db->initialized) {
		return;
	}

	search_elt.keylen = keylen;
	search_elt.p = key;
	/* First search in ops queue */
	if ((op = g_hash_table_lookup (db->ops_hash, &search_elt)) != NULL) {
		op->op = FILE_OP_DELETE;
		if (op->ref > 0) {
			op->ref--;
		}
		return;
	}
	/* Get filename */
	if (!get_file_name (db, key, keylen, filebuf, sizeof (filebuf))) {
		return;
	}

	if (db->do_ref) {
		if ((fd = file_open_fd (filebuf, &len, O_RDWR)) == -1) {
			return;
		}
		if ((ref = file_get_ref (fd)) <= 1) {
			/* Refcount is not enough, remove file */
			close (fd);
			unlink (filebuf);
		}
		else {
			/* Decrease ref */
			lseek (fd, 0, SEEK_SET);
			file_set_ref (fd, --ref);
		}
		return;
	}

	unlink (filebuf);
}

static gboolean
rspamd_file_incref (struct rspamd_kv_backend *backend,
	gpointer key,
	guint keylen)
{
	struct rspamd_file_backend *db = (struct rspamd_file_backend *)backend;
	gchar filebuf[PATH_MAX];
	struct rspamd_kv_element search_elt;
	struct file_op *op;
	gsize len;
	gint fd;
	guint32 ref;

	if (!db->initialized) {
		return FALSE;
	}
	if (!db->do_ref) {
		return TRUE;
	}

	search_elt.keylen = keylen;
	search_elt.p = key;
	/* First search in ops queue */
	if ((op = g_hash_table_lookup (db->ops_hash, &search_elt)) != NULL) {
		op->ref++;
		if (op->op == FILE_OP_DELETE) {
			op->op = FILE_OP_INSERT;
		}
		return TRUE;
	}

	/* Get filename */
	if (!get_file_name (db, key, keylen, filebuf, sizeof (filebuf))) {
		return FALSE;
	}

	if ((fd = file_open_fd (filebuf, &len, O_RDWR)) == -1) {
		return FALSE;
	}

	ref = file_get_ref (fd);

	/* Decrease ref */
	lseek (fd, 0, SEEK_SET);

	if (file_set_ref (fd, ++ref)) {
		close (fd);
		return TRUE;
	}
	else {
		close (fd);
		return FALSE;
	}
}

static void
rspamd_file_destroy (struct rspamd_kv_backend *backend)
{
	struct rspamd_file_backend *db = (struct rspamd_file_backend *)backend;

	if (db->initialized) {
		file_process_queue (backend);
		g_free (db->filename);
		g_free (db->dirname);
		g_queue_free (db->ops_queue);
		g_hash_table_unref (db->ops_hash);
		g_slice_free1 (sizeof (struct rspamd_file_backend), db);

		/* Sync again */
		sync ();
	}
}

/* Create new file backend */
struct rspamd_kv_backend *
rspamd_kv_file_new (const gchar *filename,
	guint sync_ops,
	guint levels,
	gboolean do_fsync,
	gboolean do_ref)
{
	struct rspamd_file_backend *new;
	struct stat st;
	gchar *dirname;

	if (filename == NULL) {
		return NULL;
	}

	dirname = g_path_get_dirname (filename);
	if (dirname == NULL || stat (dirname, &st) == -1 || !S_ISDIR (st.st_mode)) {
		/* Inaccessible path */
		if (dirname != NULL) {
			g_free (dirname);
		}
		msg_err ("invalid file: %s", filename);
		return NULL;
	}

	new = g_slice_alloc0 (sizeof (struct rspamd_file_backend));
	new->dirname = dirname;
	new->dirlen = strlen (dirname);
	new->filename = g_strdup (filename);
	new->sync_ops = sync_ops;
	new->levels = levels;
	new->do_fsync = do_fsync;
	new->do_ref = do_ref;
	new->ops_queue = g_queue_new ();
	new->ops_hash = g_hash_table_new (kv_elt_hash_func, kv_elt_compare_func);

	/* Init callbacks */
	new->init_func = rspamd_file_init;
	new->insert_func = rspamd_file_insert;
	new->lookup_func = rspamd_file_lookup;
	new->delete_func = rspamd_file_delete;
	new->replace_func = rspamd_file_replace;
	new->sync_func = file_process_queue;
	new->incref_func = rspamd_file_incref;
	new->destroy_func = rspamd_file_destroy;

	return (struct rspamd_kv_backend *)new;
}

