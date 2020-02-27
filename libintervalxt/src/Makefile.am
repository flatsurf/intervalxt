lib_LTLIBRARIES = libintervalxt.la

libintervalxt_la_SOURCES =                         \
	component.cc                               \
	component_state.cc                         \
	connection.cc                              \
	decomposition_state.cc                     \
	decomposition_step.cc                      \
	dynamical_decomposition.cc                 \
	half_edge.cc                               \
	induction_step.cc                          \
	interval_exchange_transformation.cc        \
	label.cc                                   \
	lengths_with_connections.cc                \
	rational_linear_subspace.cc                \
	separatrix.cc

libintervalxt_la_LDFLAGS = -version-info $(libintervalxt_version_info)

nobase_pkginclude_HEADERS =                                       \
	../intervalxt/cereal.hpp                                  \
	../intervalxt/component.hpp                               \
	../intervalxt/connection.hpp                              \
	../intervalxt/cppyy.hpp                                   \
	../intervalxt/decomposition_step.hpp                      \
	../intervalxt/dynamical_decomposition.hpp                 \
	../intervalxt/erased/boost.hpp                            \
	../intervalxt/erased/cereal.hpp                           \
	../intervalxt/erased/detail/cereal.hpp                    \
	../intervalxt/erased/detail/serializable.hpp              \
	../intervalxt/erased/detail/unparen.hpp                   \
	../intervalxt/erased/forward.hpp                          \
	../intervalxt/erased/serializable.hpp                     \
	../intervalxt/external/gmpxxll/mpz_class.hpp              \
	../intervalxt/external/spimpl/spimpl.h                    \
	../intervalxt/fmt.hpp                                     \
	../intervalxt/forward.hpp                                 \
	../intervalxt/half_edge.hpp                               \
	../intervalxt/induction_step.hpp                          \
	../intervalxt/interval_exchange_transformation.hpp        \
	../intervalxt/label.hpp                                   \
	../intervalxt/length.hpp                                  \
	../intervalxt/length_with_coefficients.hpp                \
	../intervalxt/lengths.hpp                                 \
	../intervalxt/sample/arithmetic.hpp                       \
	../intervalxt/sample/cereal.hpp                           \
	../intervalxt/sample/detail/lengths.ipp                   \
	../intervalxt/sample/e-antic-arithmetic.hpp               \
	../intervalxt/sample/exact-real-arithmetic.hpp            \
	../intervalxt/sample/lengths.hpp                          \
	../intervalxt/sample/mpz-arithmetic.hpp                   \
	../intervalxt/sample/rational-arithmetic.hpp              \
	../intervalxt/separatrix.hpp                              \
	../intervalxt/serializable.hpp


noinst_HEADERS =                                                  \
	impl/component.impl.hpp                                   \
	impl/component_state.hpp                                  \
	impl/connection.impl.hpp                                  \
	impl/decomposition_state.hpp                              \
	impl/dynamical_decomposition.impl.hpp                     \
	impl/forward.hpp                                          \
	impl/half_edge.impl.hpp                                   \
	impl/interval.hpp                                         \
	impl/interval_exchange_transformation.impl.hpp            \
	impl/lengths_with_connections.hpp                         \
	impl/rational_linear_subspace.hpp                         \
	impl/rational_linear_subspace.impl.hpp                    \
	impl/separatrix.impl.hpp                                  \
	util/assert.ipp

$(builddir)/../intervalxt/intervalxt.hpp: $(srcdir)/../intervalxt/intervalxt.hpp.in Makefile
	mkdir -p $(builddir)/../intervalxt
	sed -e 's,[@]libdir[@],$(libdir),g' < $< > $@

# Since we generate intervalxt.hpp, we need to search $(builddir) during out-of-tree builds aka VPATH builds
VPATH = $(srcdir) $(builddir)
# Search intervalxt.hpp and config.h in the builddir during VPATH builds
AM_CPPFLAGS = -I../intervalxt
# Generate intervalxt.hpp before performing any compilation
BUILT_SOURCES = ../intervalxt/intervalxt.hpp
# Distribute intervalxt.hpp.in as part of the tarball, but not intervalxt.hpp
EXTRA_DIST = ../intervalxt/intervalxt.hpp.in
# We vendor the header-only library rx-ranges which we use in some of our .cc
# files. rx-ranges is basically not packaged in any distribution since it is a
# header-only library that people always vendor into their source tree.
EXTRA_DIST += external/rx-ranges/include/rx/ranges.hpp

generatedincludedir = $(includedir)/intervalxt
nodist_generatedinclude_HEADERS = ../intervalxt/intervalxt.hpp

CLEANFILES = ../intervalxt/intervalxt.hpp
