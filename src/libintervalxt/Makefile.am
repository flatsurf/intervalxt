lib_LTLIBRARIES = libintervalxt.la

libintervalxt_la_SOURCES =               \
    fdiv.cc                              \
    interval_exchange_transformation.cc \
		label.cc

libintervalxt_la_LDFLAGS = -version-info $(libintervalxt_version_info)

nobase_include_HEADERS =                                  \
    intervalxt/interval_exchange_transformation.hpp       \
    intervalxt/label.hpp                                  \
    intervalxt/interval.hpp                               \
    intervalxt/rat_linspace.hpp                           \
    intervalxt/forward.hpp                                \
    intervalxt/cppyy.hpp

$(builddir)/intervalxt/intervalxt.hpp: $(srcdir)/intervalxt/intervalxt.hpp.in Makefile
	mkdir -p $(builddir)/libintervalxt
	sed -e 's,[@]libdir[@],$(libdir),g' < $< > $@

# Since we generate intervalxt.hpp, we need to search $(builddir) during out-of-tree builds aka VPATH builds
VPATH = $(srcdir) $(builddir)
# Generate intervalxt.hpp before performing any compilation
BUILT_SOURCES = intervalxt/intervalxt.hpp
# Distribute intervalxt.hpp.in as part of the tarball, but not intervalxt.hpp
EXTRA_DIST = intervalxt/intervalxt.hpp.in
nodist_pkginclude_HEADERS = intervalxt/intervalxt.hpp

CLEANFILES = intervalxt/intervalxt.hpp
