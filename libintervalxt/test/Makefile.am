if HAVE_GOOGLETEST
  check_PROGRAMS = dynamical_decomposition rational_linear_subspace interval_exchange_transformation induction saf boshernitzan induction_benchmark cereal
  TESTS = $(check_PROGRAMS)
endif

rational_linear_subspace_SOURCES = rational_linear_subspace.test.cc main.hpp
interval_exchange_transformation_SOURCES = interval_exchange_transformation.test.cc main.hpp
induction_SOURCES = induction.test.cc main.hpp
saf_SOURCES = saf.test.cc main.hpp
boshernitzan_SOURCES = boshernitzan.test.cc main.hpp
induction_benchmark_SOURCES = induction.benchmark.cc main.hpp
cereal_SOURCES = cereal.test.cc main.hpp
dynamical_decomposition_SOURCES = dynamical_decomposition.test.cc main.cc

# We vendor the header-only library Cereal (serialization with C++ to be able
# to run the tests even when cereal is not installed.
# Cereal is basically not packaged in any distribution since it is a
# header-only library that people always vendor into their source tree.
EXTRA_DIST = external/cereal/include
# We vendor the header-only catch2 testing framework
EXTRA_DIST += external/catch2/single_include/catch2

@VALGRIND_CHECK_RULES@

AM_CPPFLAGS = -I $(srcdir)/../src -I $(builddir)/../src -isystem $(srcdir)/external/cereal/include -isystem $(srcdir)/external/catch2/single_include/catch2/
AM_LDFLAGS = $(builddir)/../src/libintervalxt.la -lgtest -lbenchmark -lrt -lpthread
# we use gmpxx; its header is using gmp directly so we need to link it in as well
AM_LDFLAGS += -lgmpxx -lgmp
