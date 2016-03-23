AM_CPPFLAGS = @VMOD_INCLUDES@ -Wall -Werror

vmoddir = @VMOD_DIR@
vmod_LTLIBRARIES = libvmod_memcached.la

libvmod_memcached_la_CFLAGS = @VMOD_INCLUDES@ $(LIBMEMCACHED_CFLAGS)
libvmod_memcached_la_LDFLAGS = -module -export-dynamic -avoid-version -shared \
	$(LIBMEMCACHED_LIBS)

libvmod_memcached_la_SOURCES = \
	vmod_memcached.c
nodist_libvmod_memcached_la_SOURCES = \
	vcc_if.c \
	vcc_if.h

$(libvmod_memcached_la_OBJECTS): vcc_if.h

vcc_if.c vcc_if.h: @VMODTOOL@ $(top_srcdir)/src/vmod_memcached.vcc
	@VMODTOOL@ $(top_srcdir)/src/vmod_memcached.vcc

VMOD_TESTS = $(top_srcdir)/src/tests/*.vtc

.PHONY: $(VMOD_TESTS)

$(top_srcdir)/src/tests/*.vtc:
	$(top_srcdir)/src/tests/runner @VARNISHTEST@ -Dvarnishd=@VARNISHD@ -Dvmod_topbuild=$(abs_top_builddir) $@

check: $(VMOD_TESTS)

EXTRA_DIST = \
	vmod_memcached.vcc \
	tests/runner \
	$(VMOD_TESTS)

CLEANFILES = $(builddir)/vcc_if.c \
	$(builddir)/vcc_if.h \
	$(builddir)/vmod_memcached.man.rst \
	$(builddir)/vmod_memcached.rst
