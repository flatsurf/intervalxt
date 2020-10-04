lib_LTLIBRARIES = libintervalxt.la

libintervalxt_la_SOURCES =                         \
	component.cc                               \
	connection.cc                              \
	decomposition_step.cc                      \
	dynamical_decomposition.cc                 \
	half_edge.cc                               \
	implementation_of_decomposition.cc         \
	induction_step.cc                          \
	interval_exchange_transformation.cc        \
	label.cc                                   \
	lengths_with_connections.cc                \
	rational_linear_subspace.cc                \
	similarity_tracker.cc                      \
	separatrix.cc

libintervalxt_la_LDFLAGS = -version-info $(libintervalxt_version_info)

nobase_pkginclude_HEADERS =                                       \
	../intervalxt/cereal.hpp                                  \
	../intervalxt/component.hpp                               \
	../intervalxt/connection.hpp                              \
	../intervalxt/copyable.hpp                                \
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
	../intervalxt/intervalxt.hpp                              \
	../intervalxt/label.hpp                                   \
	../intervalxt/length.hpp                                  \
	../intervalxt/lengths.hpp                                 \
	../intervalxt/movable.hpp                                 \
	../intervalxt/sample/cereal.hpp                           \
	../intervalxt/sample/coefficients.hpp                     \
	../intervalxt/sample/detail/lengths.ipp                   \
	../intervalxt/sample/element_coefficients.hpp             \
	../intervalxt/sample/element_floor_division.hpp           \
	../intervalxt/sample/floor_division.hpp                   \
	../intervalxt/sample/integer_coefficients.hpp             \
	../intervalxt/sample/integer_floor_division.hpp           \
	../intervalxt/sample/lengths.hpp                          \
	../intervalxt/sample/mpq_coefficients.hpp                 \
	../intervalxt/sample/mpq_floor_division.hpp               \
	../intervalxt/sample/mpz_coefficients.hpp                 \
	../intervalxt/sample/mpz_floor_division.hpp               \
	../intervalxt/sample/renf_elem_coefficients.hpp           \
	../intervalxt/sample/renf_elem_floor_division.hpp         \
	../intervalxt/separatrix.hpp                              \
	../intervalxt/serializable.hpp                            \
	../intervalxt/shared.hpp


noinst_HEADERS =                                                  \
	impl/component.impl.hpp                                   \
	impl/connection.impl.hpp                                  \
	impl/decomposition_state.hpp                              \
	impl/dynamical_decomposition.impl.hpp                     \
	impl/forward.hpp                                          \
	impl/half_edge.impl.hpp                                   \
	impl/implementation_of_decomposition.hpp                  \
	impl/interval.hpp                                         \
	impl/interval_exchange_transformation.impl.hpp            \
	impl/lengths_with_connections.hpp                         \
	impl/rational_linear_subspace.hpp                         \
	impl/separatrix.impl.hpp                                  \
	impl/similarity_tracker.hpp                               \
	util/assert.ipp

$(builddir)/../intervalxt/local.hpp: $(srcdir)/../intervalxt/local.hpp.in Makefile
	mkdir -p $(builddir)/../intervalxt
	sed -e 's,[@]libdir[@],$(libdir),g' < $< > $@

# Since we generate local.hpp, we need to search $(builddir) during out-of-tree builds aka VPATH builds
VPATH = $(srcdir) $(builddir)
# Search local.hpp and config.h in the builddir during VPATH builds
AM_CPPFLAGS = -I../intervalxt
# Generate local.hpp before performing any compilation
BUILT_SOURCES = ../intervalxt/local.hpp
# Distribute local.hpp.in as part of the tarball, but not local.hpp
EXTRA_DIST = ../intervalxt/local.hpp.in
# We vendor the header-only library rx-ranges which we use in some of our .cc
# files. rx-ranges is basically not packaged in any distribution since it is a
# header-only library that people always vendor into their source tree.
EXTRA_DIST += external/rx-ranges/include/rx/ranges.hpp

generatedincludedir = $(includedir)/intervalxt
nodist_generatedinclude_HEADERS = ../intervalxt/local.hpp

CLEANFILES = ../intervalxt/local.hpp
