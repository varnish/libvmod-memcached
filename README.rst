==============
vmod_memcached
==============

------------------------
Varnish Memcached Module
------------------------

:Author: Aaron Stone
:Author: Federico G. Schwindt
:Date: 2014-09-12
:Version: 0.2
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
        Set the list of memcached servers available for requests handled
        by this VCL.

        libmemcached versions prior to 0.49 use a comma separated list
        of one or more "hostname[:port]" servers.

        Newer versions also support a custom syntax which uses
        "--SERVER=ip:port" to specify a server.
        See http://docs.libmemcached.org/libmemcached_configuration.html
        for all supported options.

        Please note there is no error checking for this string. If
        it's wrong you won't get an error but this module won't work.
        Check first if the connection string is valid.
Example
        ::

                # Old format.
                memcached.servers("hostA,hostB:1234");
                # New format.
                memcached.servers("--SERVER=hostA --SERVER=hostB:1234");

get
---

Prototype
        ::

                get(STRING key)
Return value
        STRING
Description
        Retrieve key from memcached and return the value.
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
        Increment key by offset and return the new value. If key is not
        set return 0.
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
        Decrement key by offset and return the new value. If key is not
        set return 0.
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
 ./configure VARNISHSRC=DIR [VMODDIR=DIR] [PKG_CONFIG=PATH] [LIBMEMCACHED_CFLAGS=COMPILE] [LIBMEMCACHED_LIBS=LINK]

`VARNISHSRC` is the directory of the Varnish source tree for which to
compile your vmod. Both the `VARNISHSRC` and `VARNISHSRC/include`
will be added to the include search paths for your module.

Optionally you can also set the vmod install directory by adding
`VMODDIR=DIR` (defaults to the pkg-config discovered directory from your
Varnish installation).

The `configure` script uses `pkg-config` to find the libmemcached library. You
may specify the `pkg-config` binary by setting the `PKG_CONFIG` option. If you
do not wish to use `pkg-config`, you may set `LIBMEMCACHED_CFLAGS` and
`LIBMEMCACHED_LIBS` as necessary to compile and link with libmemcached.

Make targets:

* make - builds the vmod
* make install - installs your vmod in `VMODDIR`
* make check - runs the unit tests in ``src/tests/*.vtc``

SEE ALSO
========

* http://memcached.org/
* http://libmemcached.org/

HISTORY
=======

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

