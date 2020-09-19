check_PROGRAMS = rational_linear_subspace label interval_exchange_transformation dynamical_decomposition cereal
TESTS = $(check_PROGRAMS)

rational_linear_subspace_SOURCES = rational_linear_subspace.test.cc main.cc
label_SOURCES = label.test.cc main.cc
interval_exchange_transformation_SOURCES = interval_exchange_transformation.test.cc main.cc
dynamical_decomposition_SOURCES = dynamical_decomposition.test.cc main.cc
cereal_SOURCES = cereal.test.cc main.cc
coefficients_SOURCES = coefficients.test.cc main.cc

# We vendor the header-only library Cereal (serialization with C++ to be able
# to run the tests even when cereal is not installed.
# Cereal is basically not packaged in any distribution since it is a
# header-only library that people always vendor into their source tree.
EXTRA_DIST = external/cereal/include
# We vendor the header-only catch2 testing framework
EXTRA_DIST += external/catch2/single_include/catch2

@VALGRIND_CHECK_RULES@

AM_CPPFLAGS = -I $(srcdir)/../ -I $(builddir)/../ -isystem $(srcdir)/external/cereal/include -isystem $(srcdir)/external/catch2/single_include/catch2/

AM_LDFLAGS = $(builddir)/../src/libintervalxt.la -lrt -lpthread
# we use gmpxx; its header is using gmp directly so we need to link it in as well
AM_LDFLAGS += -lgmpxx -lgmp
