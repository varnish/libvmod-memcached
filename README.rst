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

config
------

Prototype
        ::

                config(STRING S)
Return value
	NONE
Description
	Configured libmemcached client according to http://docs.libmemcached.org/libmemcached_configuration.html
Example
        ::

                memcached.config("--SERVER=localhost");

get
---

Prototype
        ::

                STRING get(STRING S)
Return value
	STRING V
Description
	Retrieve key S from memcached, returns a string.
Example
        ::

                memcached.config("--SERVER=localhost");

                set resp.http.hello = memcached.get("your_memcached_key");


INSTALLATION
============

TODO: optionally specify the path to libmemcached.

The source tree is based on autotools to configure the building, and
does also have the necessary bits in place to do functional unit tests
using the varnishtest tool.

Usage::

 ./configure VARNISHSRC=DIR [VMODDIR=DIR]

`VARNISHSRC` is the directory of the Varnish source tree for which to
compile your vmod. Both the `VARNISHSRC` and `VARNISHSRC/include`
will be added to the include search paths for your module.

Optionally you can also set the vmod install directory by adding
`VMODDIR=DIR` (defaults to the pkg-config discovered directory from your
Varnish installation).

Make targets:

* make - builds the vmod
* make install - installs your vmod in `VMODDIR`
* make check - runs the unit tests in ``src/tests/*.vtc``

HISTORY
=======

The first revision of this document sketches out a rough plan for approaching a
general purpose memcached client module for Varnish. More features are sure to
be added as we go along.

This manual page is based on the template man page from libvmod-example.

COPYRIGHT
=========

This document is licensed under the same license as the
libvmod-example project. See LICENSE for details.

* Copyright (c) 2012 Aaron Stone
* Copyright (c) 2011 Varnish Software
