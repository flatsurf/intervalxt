lib_LTLIBRARIES = libintervalxt.la

libintervalxt_la_SOURCES =                         \
	rational_linear_subspace.cc                \
	label.cc                                   \
	interval_exchange_transformation.cc        \
	dynamical_decomposition.cc                 \
	component.cc                               \
	lengths_with_connections.cc                \
	component_state.cc                         \
	connection.cc                              \
	separatrix.cc                              \
	half_edge.cc                               \
	decomposition_step.cc                      \
	decomposition_state.cc

libintervalxt_la_LDFLAGS = -version-info $(libintervalxt_version_info)

nobase_include_HEADERS =                                          \
	../intervalxt/forward.hpp                                 \
	../intervalxt/label.hpp                                   \
	../intervalxt/interval_exchange_transformation.hpp        

$(builddir)/../intervalxt/intervalxt.hpp: $(srcdir)/../intervalxt/intervalxt.hpp.in Makefile
	mkdir -p $(builddir)/../intervalxt
	sed -e 's,[@]libdir[@],$(libdir),g' < $< > $@

AM_CPPFLAGS = -isystem $(srcdir)/external/range-v3/include/

# Since we generate intervalxt.hpp, we need to search $(builddir) during out-of-tree builds aka VPATH builds
VPATH = $(srcdir) $(builddir)
# Generate intervalxt.hpp before performing any compilation
BUILT_SOURCES = ../intervalxt/intervalxt.hpp
# Distribute intervalxt.hpp.in as part of the tarball, but not intervalxt.hpp
EXTRA_DIST = ../intervalxt/intervalxt.hpp.in
generatedincludedir = $(includedir)/intervalxt
nodist_generatedinclude_HEADERS = ../intervalxt/intervalxt.hpp

CLEANFILES = ../intervalxt/intervalxt.hpp
