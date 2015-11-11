#define _GNU_SOURCE

#include <libmemcached/memcached.h>
#include <libmemcached/util.h>

#include "vrt.h"
#include "bin/varnishd/cache.h"

#include "vcc_if.h"

#include <string.h>
#include <time.h>


#define POOL_MAX_CONN_STR     "40"
#define POOL_MAX_CONN_PREFIX  "--POOL-MAX="
#define POOL_MAX_CONN_PARAM   " " POOL_MAX_CONN_PREFIX POOL_MAX_CONN_STR
#define POOL_TIMEOUT_MSEC     3000
#define POOL_ERROR_INT        -1
#define POOL_ERROR_STRING     NULL


typedef struct
{
	memcached_pool_st  *pool;
	long                pool_timeout_msec;
	int                 error_int;
	char               *error_str;
	char                error_str_value[128];
}
vmod_mc_vcl_settings;


static void free_mc_vcl_settings(void *data)
{
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)data;

	memcached_pool_destroy(settings->pool);

	free(settings);
}

int init_function(struct vmod_priv *priv, const struct VCL_conf *conf)
{
	vmod_mc_vcl_settings *settings;

	settings=calloc(1, sizeof(vmod_mc_vcl_settings));

	AN(settings);

	settings->pool_timeout_msec = POOL_TIMEOUT_MSEC;
	settings->error_int = POOL_ERROR_INT;
	settings->error_str = POOL_ERROR_STRING;

	priv->priv=settings;
	priv->free=free_mc_vcl_settings;

	return 0;
}

static memcached_st *get_memcached(struct sess *sp, vmod_mc_vcl_settings *settings)
{
	memcached_return_t rc;
	memcached_st *mc;
	struct timespec wait;

	AN(settings->pool);

	wait.tv_nsec = 1000 * 1000 * (settings->pool_timeout_msec % 1000);
	wait.tv_sec = settings->pool_timeout_msec / 1000;

	mc = memcached_pool_fetch(settings->pool, &wait, &rc);

	if(rc == MEMCACHED_SUCCESS)
	{
		return mc;
	}

	return NULL;
}

static void release_memcached(struct sess *sp, vmod_mc_vcl_settings *settings, memcached_st *mc)
{
	memcached_pool_release(settings->pool, mc);
}

void vmod_servers(struct sess *sp, struct vmod_priv *priv, const char *config)
{
	char error_buf[256];
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;

	AZ(settings->pool);

	if(strcasestr(config, POOL_MAX_CONN_PREFIX))
	{
		settings->pool = memcached_pool(config, strlen(config));

		VSL(SLT_Debug, 0, "memcached pool config '%s'", config);
	}
	else
	{
		size_t pool_len = strlen(config) + strlen(POOL_MAX_CONN_PARAM);
		char *pool_str = malloc(pool_len + 1);

		strcpy(pool_str, config);
		strcat(pool_str, POOL_MAX_CONN_PARAM);

		settings->pool = memcached_pool(pool_str, pool_len);

		VSL(SLT_Debug, 0, "memcached pool config '%s'", pool_str);

		free(pool_str);
	}

	if(!settings->pool)
	{
		libmemcached_check_configuration(config, strlen(config), error_buf, sizeof(error_buf));
		VSL(SLT_Error, 0, "memcached servers() error");
		VSL(SLT_Error, 0, "%s", error_buf);
		AN(settings->pool);
	}
}

void vmod_error_string(struct sess *sp, struct vmod_priv *priv, const char *string)
{
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;

	strncpy(settings->error_str_value, string, sizeof(settings->error_str_value));
	settings->error_str_value[sizeof(settings->error_str_value) - 1] = '\0';

	settings->error_str = settings->error_str_value;
}

void vmod_pool_timeout_msec(struct sess *sp, struct vmod_priv *priv, int timeout)
{
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;

	settings->pool_timeout_msec = timeout;
}

void vmod_set(struct sess *sp, struct vmod_priv *priv, const char *key,
	const char *value, int expiration, int flags)
{
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;
	memcached_st *mc = get_memcached(sp, settings);

	if(mc)
	{
		memcached_set(mc, key, strlen(key), value, strlen(value), expiration, flags);
		release_memcached(sp, settings, mc);
	}
}

const char *vmod_get(struct sess *sp, struct vmod_priv *priv, const char *key)
{
	size_t len;
	uint32_t flags;
	memcached_return rc;
	char *p, *value;
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;
	memcached_st *mc = get_memcached(sp, settings);

	if (!mc)
	{
		return settings->error_str;
	}

	value = memcached_get(mc, key, strlen(key), &len, &flags, &rc);

	release_memcached(sp, settings, mc);

	if(rc || !value)
	{
		return settings->error_str;
	}

        p = WS_Dup(sp->ws, value);

	free(value);

	return p;
}

int vmod_incr(struct sess *sp, struct vmod_priv *priv, const char *key, int offset)
{
	memcached_return_t rc;
	uint64_t value = 0;
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;
	memcached_st *mc = get_memcached(sp, settings);

	if (!mc)
	{
		return settings->error_int;
	}

	memcached_increment(mc, key, strlen(key), offset, &value);

	rc = memcached_last_error(mc);

	release_memcached(sp, settings, mc);

	if(rc)
	{
		return settings->error_int;
	}

	return (int)value;
}

int vmod_decr(struct sess *sp, struct vmod_priv *priv, const char *key, int offset)
{
	memcached_return_t rc;
	uint64_t value = 0;
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;
	memcached_st *mc = get_memcached(sp, settings);

	if (!mc)
	{
		return settings->error_int;
	}

	memcached_decrement(mc, key, strlen(key), offset, &value);

	rc = memcached_last_error(mc);

	release_memcached(sp, settings, mc);

	if(rc)
	{
		return settings->error_int;
	}

	return (int)value;
}

int vmod_incr_set(struct sess *sp, struct vmod_priv *priv,
	const char *key, int offset, int initial, int expiration)
{
	memcached_return_t rc;
	uint64_t value = 0;
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;
	memcached_st *mc = get_memcached(sp, settings);

	if (!mc)
	{
		return settings->error_int;
	}

	memcached_increment_with_initial(mc, key, strlen(key), offset,
		initial, expiration, &value);

	rc = memcached_last_error(mc);

	release_memcached(sp, settings, mc);

	if(rc)
	{
		return settings->error_int;
	}

	return (int)value;
}

int vmod_decr_set(struct sess *sp, struct vmod_priv *priv,
	const char *key, int offset, int initial, int expiration)
{
	memcached_return_t rc;
	uint64_t value = 0;
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;
	memcached_st *mc = get_memcached(sp, settings);

	if (!mc)
	{
		return settings->error_int;
	}

	memcached_decrement_with_initial(mc, key, strlen(key), offset,
		initial, expiration, &value);

	rc = memcached_last_error(mc);

	release_memcached(sp, settings, mc);

	if(rc)
	{
		return settings->error_int;
	}

	return (int)value;
}
