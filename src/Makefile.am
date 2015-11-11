
vmoddir = $(VMODDIR)
vmod_LTLIBRARIES = libvmod_memcached.la

libvmod_memcached_la_CFLAGS = -I$(VARNISHSRC)/include -I$(VARNISHSRC) \
	$(LIBMEMCACHED_CFLAGS)
libvmod_memcached_la_LDFLAGS = -module -export-dynamic -avoid-version -shared \
	$(LIBMEMCACHED_LIBS)

libvmod_memcached_la_SOURCES = \
	vmod_memcached.c
nodist_libvmod_memcached_la_SOURCES = \
	vcc_if.c \
	vcc_if.h


VMOD_TESTS = tests/*.vtc

$(libvmod_memcached_la_OBJECTS): vcc_if.h

vcc_if.c vcc_if.h: $(VARNISHSRC)/lib/libvmod_std/vmod.py $(top_srcdir)/src/vmod_memcached.vcc
	$(PYTHON) $(VARNISHSRC)/lib/libvmod_std/vmod.py $(top_srcdir)/src/vmod_memcached.vcc

check:
	tests/runner $(VARNISHSRC)/bin/varnishtest/varnishtest \
	    -Dvarnishd=$(VARNISHSRC)/bin/varnishd/varnishd \
	    -Dvmod_topbuild=$(abs_top_builddir) $(VMOD_TESTS)

EXTRA_DIST = \
	vmod_memcached.vcc \
	$(VMOD_TESTS)

CLEANFILES = $(builddir)/vcc_if.c $(builddir)/vcc_if.h
