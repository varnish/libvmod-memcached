#include <libmemcached/memcached.h>
#include <libmemcached/util.h>

#include "vrt.h"
#include "cache/cache.h"

#include "vcc_if.h"

#include <time.h>


typedef struct
{
	memcached_pool_st *pool;
	long pool_timeout_msec;
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

	priv->priv=settings;
	priv->free=free_mc_vcl_settings;

	return 0;
}

memcached_st *get_memcached(const struct vrt_ctx *ctx, vmod_mc_vcl_settings *settings)
{
	memcached_return_t rc;
	memcached_st *mc;
	struct timespec wait;

	AN(settings->pool);

	wait.tv_nsec = 0;
	wait.tv_sec = 2;

	mc = memcached_pool_fetch(settings->pool, &wait, &rc);

	if(rc == MEMCACHED_SUCCESS)
	{
		return mc;
	}

	return NULL;
}

void release_memcached(const struct vrt_ctx *ctx, vmod_mc_vcl_settings *settings, memcached_st *mc)
{
	memcached_pool_release(settings->pool, mc);
}

VCL_VOID vmod_servers(const struct vrt_ctx *ctx, struct vmod_priv *priv, VCL_STRING config)
{
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;

	settings->pool = memcached_pool(config, strlen(config));
}

VCL_VOID vmod_set(const struct vrt_ctx *ctx, struct vmod_priv *priv, VCL_STRING key,
	VCL_STRING value, VCL_INT expiration, VCL_INT flags)
{
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;
	memcached_st *mc = get_memcached(ctx, settings);

	if(mc)
	{
		memcached_set(mc, key, strlen(key), value, strlen(value), expiration, flags);
		release_memcached(ctx, settings, mc);
	}
}

VCL_STRING vmod_get(const struct vrt_ctx *ctx, struct vmod_priv *priv, VCL_STRING key)
{
	size_t len;
	uint32_t flags;
	memcached_return rc;
	char *p, *value;
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;
	memcached_st *mc = get_memcached(ctx, settings);

	if (!mc)
	{
		return NULL;
	}

	value = memcached_get(mc, key, strlen(key), &len, &flags, &rc);

	release_memcached(ctx, settings, mc);

	if (!value)
	{
		return NULL;
	}

	p = WS_Copy(ctx->ws, value, -1);
	free(value);

	return p;
}

VCL_INT __match_proto__(td_memcached_incr)
vmod_incr(const struct vrt_ctx *ctx, struct vmod_priv *priv, VCL_STRING key, VCL_INT offset)
{
	uint64_t value = 0;
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;
	memcached_st *mc = get_memcached(ctx, settings);

	if (!mc)
	{
		return 0;
	}

	memcached_increment(mc, key, strlen(key), offset, &value);

	release_memcached(ctx, settings, mc);

	return (int)value;
}

VCL_INT vmod_decr(const struct vrt_ctx *ctx, struct vmod_priv *priv, VCL_STRING key, VCL_INT offset)
{
	uint64_t value = 0;
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;
	memcached_st *mc = get_memcached(ctx, settings);

	if (!mc)
	{
		return 0;
	}

	memcached_decrement(mc, key, strlen(key), offset, &value);

	release_memcached(ctx, settings, mc);

	return (int)value;
}

VCL_INT vmod_incr_set(const struct vrt_ctx *ctx, struct vmod_priv *priv,
	VCL_STRING key, VCL_INT offset, VCL_INT initial, VCL_INT expiration)
{
	uint64_t value = 0;
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;
	memcached_st *mc = get_memcached(ctx, settings);

	if (!mc)
	{
		return 0;
	}

	memcached_increment_with_initial(mc, key, strlen(key), offset,
		initial, expiration, &value);

	release_memcached(ctx, settings, mc);

	return (int)value;
}

VCL_INT vmod_decr_set(const struct vrt_ctx *ctx, struct vmod_priv *priv,
	VCL_STRING key, VCL_INT offset, VCL_INT initial, VCL_INT expiration)
{
	uint64_t value = 0;
	vmod_mc_vcl_settings *settings = (vmod_mc_vcl_settings*)priv->priv;
	memcached_st *mc = get_memcached(ctx, settings);

	if (!mc)
	{
		return (0);
	}

	memcached_decrement_with_initial(mc, key, strlen(key), offset,
		initial, expiration, &value);

	release_memcached(ctx, settings, mc);

	return (int)value;
}
