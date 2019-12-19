lib_LTLIBRARIES = libintervalxt.la

libintervalxt_la_SOURCES =                         \
	rational_linear_subspace.cc                \
	label.cc                                   \
	interval_exchange_transformation.cc
#	length_arithmetic/mpz_class.cc             \
#	length_arithmetic/mpq_class.cc             \
#	length_arithmetic/renf_elem_class.cc       \
#	length_arithmetic/integral.cc              \
#	id.cc                                      \
#	external/unique-factory/unique_factory.hpp

libintervalxt_la_LDFLAGS = -version-info $(libintervalxt_version_info)

nobase_include_HEADERS =                                          \
	../intervalxt/forward.hpp                                 \
	../intervalxt/label.hpp                                   \
	../intervalxt/interval_exchange_transformation.hpp        
#	../intervalxt/detail/interval_exchange_transformation.ipp \
#	../intervalxt/detail/dynamical_decomposition.ipp          \
#	../intervalxt/detail/decomposition_step.ipp               \
#	../intervalxt/detail/component.ipp                        \
#	../intervalxt/detail/connection.ipp                       \
#	../intervalxt/detail/maybe_connection.ipp                 \
#	../intervalxt/detail/label.ipp                            \
#	../intervalxt/detail/id.hpp                               \
#	../intervalxt/length.hpp                                  \
#	../intervalxt/detail/length.ipp                           \
#	../intervalxt/cppyy.hpp                                   \
#	../intervalxt/induction_step.hpp                          \
#	../intervalxt/decomposition_step.hpp                      \
#	../intervalxt/dynamical_decomposition.hpp                 \
#	../intervalxt/component.hpp                               \
#	../intervalxt/connection.hpp                              \
#	../intervalxt/maybe_connection.hpp                        \
#	../intervalxt/external/spimpl/spimpl.h                    \
#	../intervalxt/external/gmpxxll/mpz_class.hpp              \
#	../intervalxt/cereal.hpp

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
