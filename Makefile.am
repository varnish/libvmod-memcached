ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src

DISTCHECK_CONFIGURE_FLAGS = \
	VMOD_DIR='$${libdir}/varnish/vmods'

EXTRA_DIST = README.rst LICENSE

dist_man_MANS = vmod_memcached.3
MAINTAINERCLEANFILES = $(dist_man_MANS)

vmod_memcached.3: README.rst
if HAVE_RST2MAN
	${RST2MAN} README.rst $@
else
	@echo "========================================"
	@echo "You need rst2man installed to make dist"
	@echo "========================================"
	@false
endif
