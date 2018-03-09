This is a running log of changes to libvmod-memcached.

libvmod-memcached 1.0.1 (2017-03-07)
------------------------------------

 * Minor fixes.
 * Update RPM dependencies.

This release was tested with Varnish Cache 4.1.9 and trunk (2018-03-07)

libvmod-memcached 1.0.0 (2016-03-23)
------------------------------------

This is the first documented source release of vmod-memcached.

Recent changes:
* Semantic versioning scheme introduced.
* Travis CI used for running tests.

This release was tested with Varnish Cache 4.1.2.

It requires a libmemcached version higher than 0.53 to build.

Maintenance and development of this module is now handled by Varnish Software.

libvmod-memcached 0.3 (unknown)
-------------------------------

Refactored with the following goals:

* Remove pthread_specific functionality since its not vcl.reload safe
* Change one connection per thread to a shared connection pool
* Better error detection

The original version of this vmod was written by Aaron Stone.
