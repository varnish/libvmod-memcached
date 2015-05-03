==============
vmod_memcached
==============

------------------------
Varnish Memcached Module
------------------------

:Author: Aaron Stone
:Author: Federico G. Schwindt
:Author: Reza Naghibi
:Date: 2015-04-24
:Version: 0.3
:Manual section: 3

SYNOPSIS
========

import memcached;

DESCRIPTION
===========

Varnish vmod using libmemcached to access memcached servers.

FUNCTIONS
=========

servers
-------

Prototype
        ::

                servers(STRING servers)
Return value
        VOID
Description
        Set the memcached parameters for requests handled by this VCL.

        Note, the old style of a comma seperated server list is no longer
        supported.

        Syntax to specify a server: "--SERVER=ip:port"

        See http://docs.libmemcached.org/libmemcached_configuration.html
        for all supported options.

        Please note that if you supply an invalid parameter string,
        Varnish will fail to start.

        This will create a connection pool for all VCL threads to share from.
        Each active or available VCL will have its own connection pool.

        If no --POOL-MAX parameter is specified, a pool will be created with
        a maximum of 40 connection.

        Using this function outside of vcl_init or calling it twice will
        result in undefined behavior.
Example
        ::

                # 2 memcached hosts
                memcached.servers("--SERVER=hostA --SERVER=hostB:1234");

                # advanced connection options
                memcached.servers("--SERVER=10.1.1.14 --BINARY-PROTOCOL --POOL-MAX=10 --CONNECT-TIMEOUT=50 --RETRY-TIMEOUT=3");

pool_timeout_msec
-----------------

Prototype
        ::

                pool_timeout_msec(INT timeout)
Return value
        VOID
Description
        Set the maximum amount of time to wait for a connection, in
        milliseconds, if the entire connection pool is in use.

        The default value is 3000.

        Using this function outside of vcl_init will result in undefined
        behavior.
Example
        ::

                # set the pool timeout of 10 seconds
                memcached.pool_timeout_msec(10000);

error_string
------------

Prototype
        ::

                error_string(STRING value)
Return value
        VOID
Description
        Set the string that is returned on a get() error.

        The default value is an empty string.

        Using this function outside of vcl_init will result in undefined
        behavior.
Example
        ::

                memcached.error_string("_ERROR");

get
---

Prototype
        ::

                get(STRING key)
Return value
        STRING
Description
        Retrieve key from memcached and return the value.

        On error, the value of error_string() is returned.
Example
        ::

                set resp.http.value = memcached.get("key");

set
---

Prototype
        ::

                set(STRING key, STRING value, INT expiration, INT flags)
Return value
        VOID
Description
        Set key to value, with an expiration time and flags.
Example
        ::

                # Set "key" to "Hello world" with a 100s expiration.
                memcached.set("key", "Hello world", 100, 0);

incr
----

Prototype
        ::

                incr(STRING key, INT offset)
Return value
        INT
Description
        Increment key by offset and return the new value.

        If the key does not exist or an error occurs, -1 is returned.
Example
        ::

                # Increment "key" by 10 and return the new value.
                set resp.http.value = memcached.incr("key", 10);

decr
----

Prototype
        ::

                decr(STRING key, INT offset)
Return value
        INT
Description
        Decrement key by offset and return the new value.

        If the key does not exist or an error occurs, -1 is returned.
Example
        ::

                # Decrement "key" by 8 and return the new value.
                set resp.http.value = memcached.decr("key", 8);

incr_set
--------

Prototype
        ::

                incr_set(STRING key, INT offset, INT initial, INT expiration)
Return value
        INT
Description
        Increment key by offset and return the new value. If key is not
        set, key will be set to initial with an expiration time.

        This is only available when using the binary protocol.

        If an error occurs, -1 is returned.
Example
        ::

                # Increment "key" by 1 if set, otherwise set it to 10
                # with no expiration.
                set resp.http.value = memcached.incr_set("key", 1, 10, 0);

decr_set
--------

Prototype
        ::

                decr_set(STRING key, INT offset, INT initial, INT expiration)
Return value
        INT
Description
        Decrement key by offset and return the new value. If key is not
        set, key will be set to initial with an expiration time.

        This is only available when using the binary protocol.

        If an error occurs, -1 is returned.
Example
        ::

                # Decrement "key" by 1 if set, otherwise set it to 10
                # with no expiration.
                set resp.http.value = memcached.decr_set("key", 1, 10, 0);

INSTALLATION
============

If you received this package without a pre-generated configure script, you must
have the GNU Autotools installed, and can then run the 'autogen.sh' script. If
you received this package with a configure script, skip to the second
command-line under Usage to configure.

Usage::

 # Generate configure script
 ./autogen.sh

 # Execute configure script
 ./configure [PKG_CONFIG=PATH] [LIBMEMCACHED_CFLAGS=COMPILE] [LIBMEMCACHED_LIBS=LINK]

The `configure` script uses `pkg-config` to find the libmemcached library. You
may specify the `pkg-config` binary by setting the `PKG_CONFIG` option. If you
do not wish to use `pkg-config`, you may set `LIBMEMCACHED_CFLAGS` and
`LIBMEMCACHED_LIBS` as necessary to compile and link with libmemcached.

Make targets:

* make - builds the vmod
* make install - installs your vmod
* make check - runs the unit tests in ``src/tests/*.vtc``

SEE ALSO
========

* http://memcached.org/
* http://libmemcached.org/

HISTORY
=======

0.3

This VMOD was refactored with the following goals:

* Remove pthread_specific functionality since its not vcl.reload safe
* Change one connection per thread to a shared connection pool
* Better error detection

0.1

The first revision of this document sketches out a rough plan for approaching a
general purpose memcached client module for Varnish. More features are sure to
be added as we go along.

This manual page is based on the template man page from libvmod-example.

COPYRIGHT
=========

* Copyright (c) 2012 Aaron Stone
* Copyright (c) 2014 Varnish Software
* See COPYING for copyright holders and descriptions.
* See LICENSE for full copyright terms.

