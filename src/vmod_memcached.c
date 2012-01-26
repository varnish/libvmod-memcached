#include <stdlib.h>
#include <libmemcached/memcached.h>

#include "vrt.h"
#include "bin/varnishd/cache.h"

#include "vcc_if.h"

int
init_function(struct vmod_priv *priv, const struct VCL_conf *conf)
{
	return (0);
}

void
vmod_memcached_config(struct sess *sp, const char *config)
{
	// TODO:
	// memcached_st *
	// memcached(const char *string,
	//           size_t string_length);
}

void
vmod_memcached_set(struct sess *sp, const char *key, const char *value)
{
	// TODO:
	// memcached_return_t
	// memcached_set (memcached_st *ptr,
	//                const char *key,
	//                size_t key_length,
	//                const char *value,
	//                size_t value_length,
	//                time_t expiration,
	//                uint32_t flags);
}

const char *
vmod_memcached_get(struct sess *sp, const char *key)
{
	// TODO:
	// char *
	// memcached_get(memcached_st *ptr,
	//               const char *key,
	//               size_t key_length,
	//               size_t *value_length,
	//               uint32_t *flags,
	//               memcached_return_t *error);
}

const char *
vmod_memcached_incr(struct sess *sp, const char *key)
{
	// TODO:
	// memcached_return_t
	// memcached_increment (memcached_st *ptr,
	//                      const char *key, size_t key_length,
	//                      unsigned int offset,
	//                      uint64_t *value);
}

const char *
vmod_memcached_decr(struct sess *sp, const char *key)
{
	// TODO:
	// memcached_return_t
	// memcached_decrement (memcached_st *ptr,
	//                      const char *key, size_t key_length,
	//                      unsigned int offset,
	//                      uint64_t *value);
}

