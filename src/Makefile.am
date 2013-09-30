INCLUDES = -I$(VARNISHSRC)/include -I$(VARNISHSRC) $(LIBMEMCACHED_CFLAGS)

vmoddir = $(VMODDIR)
vmod_LTLIBRARIES = libvmod_memcached.la

libvmod_memcached_la_LDFLAGS = -module -export-dynamic -avoid-version $(LIBMEMCACHED_LIBS)

libvmod_memcached_la_SOURCES = \
	vcc_if.c \
	vcc_if.h \
	vmod_memcached.c

vcc_if.c vcc_if.h: $(VARNISHSRC)/lib/libvmod_std/vmod.py $(top_srcdir)/src/vmod_memcached.vcc
	@PYTHON@ $(VARNISHSRC)/lib/libvmod_std/vmod.py $(top_srcdir)/src/vmod_memcached.vcc

VMOD_TESTS = tests/*.vtc
.PHONY: $(VMOD_TESTS)

tests/*.vtc:
	tests/runner $(VARNISHSRC)/bin/varnishtest/varnishtest -Dvarnishd=$(VARNISHSRC)/bin/varnishd/varnishd -Dvmod_topbuild=$(abs_top_builddir) $@

check: $(VMOD_TESTS)

EXTRA_DIST = \
	vmod_memcached.vcc \
	$(VMOD_TESTS)

CLEANFILES = $(builddir)/vcc_if.c $(builddir)/vcc_if.h
