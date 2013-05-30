==============
vmod_memcached
==============

------------------------
Varnish Memcached Module
------------------------

:Author: Aaron Stone
:Date: 2012-01-25
:Version: 0.1
:Manual section: 3

SYNOPSIS
========

import memcached;

DESCRIPTION
===========

Varnish vmod using libmemcached to access memcached servers

Implements the basic memcached operations of get, set, incr, decr.

FUNCTIONS
=========

servers
-------

Prototype
        ::

                servers(STRING servers)
Return value
	NONE
Description
	Set the list of memcached servers available for requests handled by this VCL. The syntax is a whitespace or comma 
        separated list of one or more "hostname[:port]" items.

        If you have libmemcached > 0.49 you can use the new syntax, specified in 
        http://docs.libmemcached.org/libmemcached_configuration.html#description , which uses "--SERVER=ip:port"
        to specify a server.
        Warning: There is no error checking for this string, if it's wrong you won't get an error but memcached
        won't work as well. Check first if the connection string is valid.
Example
        ::

                memcached.servers("localhost,anotherhost:12345");
        ::

                memcached.servers("--SERVER=localhost --SERVER=anotherhost:12345");
        ::

                // with consistent hashing enabled
                memcached.servers("--SERVER=localhost --SERVER=anotherhost:12345 --DISTRIBUTION=consistent");
        ::

                // with consistent hashing enabled and namespace
                memcached.servers({"--SERVER=web1.gloople:11211 --SERVER=web2.gloople:11211 --DISTRIBUTION=consistent --NAMESPACE="memc.sess.key.""});

get
---

Prototype
        ::

                STRING get(STRING key)
Return value
	STRING V
Description
	Retrieve key from memcached, returns string value.
Example
        ::

                memcached.servers("localhost");

                set resp.http.hello = memcached.get("your_memcached_key");

set
---

Prototype
        ::

                set(STRING key, STRING value, INT expiration, INT flags)
Return value
	NONE
Description
	Set key to value, with an expiration time and flags.
Example
        ::

                memcached.servers("localhost");

                memcached.set("your_memcached_key", "Hello, World", 100, 0);

                set resp.http.hello = memcached.get("your_memcached_key");

incr
----

Prototype
        ::

                INT incr(STRING key, INT offset)
Return value
	INT
Description
	Increment key by offset, unless key is not set. Return value is 0 if not set.
Example
        ::

                memcached.servers("localhost");

                memcached.set("your_counter", "1", 100, 0);

                memcached.incr("your_counter", 10);

                set resp.http.count = memcached.incr("your_counter", 1);

		// Header value is Count: 12

decr
----

Prototype
        ::

                INT decr(STRING key, INT offset)
Return value
	INT
Description
	Decrement key by offset, unless key is not set. Return value is 0 if not set.
Example
        ::

                memcached.servers("localhost");

                memcached.set("your_counter", "10", 100, 0);

                memcached.decr("your_counter", 8);

                set resp.http.count = memcached.decr("your_counter", 1);

		// Header value is Count: 1


INSTALLATION
============

If you received this packge without a pre-generated configure script, you must
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
* See COPYING for copyright holders and descriptions.
* See LICENSE for full copyright terms.

