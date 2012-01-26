#include <stdlib.h>
#include <libmemcached/memcached.h>

#include "vrt.h"
#include "bin/varnishd/cache.h"

#include "vcc_if.h"

int
init_function(struct vmod_priv *priv, const struct VCL_conf *conf)
{
	priv->free = (vmod_priv_free_f *)memcached_free;

	return (0);
}

void
vmod_config(struct sess *sp, struct vmod_priv *priv, const char *config)
{
	priv->priv = memcached(config, strlen(config));
}

void
vmod_set(struct sess *sp, struct vmod_priv *priv, const char *key, const char *value, int expiration, int flags)
{
	memcached_return_t rc;

	rc = memcached_set(priv->priv, key, strlen(key), value, strlen(value), expiration, flags);
}

const char *
vmod_get(struct sess *sp, struct vmod_priv *priv, const char *key)
{
	size_t len;
	uint32_t flags;
	memcached_return_t rc;

	char *value = memcached_get(priv->priv, key, strlen(key), &len, &flags, &rc);

	return value;
}

int
vmod_incr(struct sess *sp, struct vmod_priv *priv, const char *key, int offset)
{
	uint64_t value;
	memcached_return_t rc;

	rc = memcached_increment(priv->priv, key, strlen(key), offset, &value);

	return (int)value;
}

int
vmod_decr(struct sess *sp, struct vmod_priv *priv, const char *key, int offset)
{
	uint64_t value;
	memcached_return_t rc;

	rc = memcached_decrement(priv->priv, key, strlen(key), offset, &value);

	return (int)value;
}

