if HAVE_GOOGLETEST
  check_PROGRAMS = rational_linear_subspace interval_exchange_transformation induction induction_benchmark cereal
  TESTS = $(check_PROGRAMS)
endif

interval_exchange_transformation_SOURCES = interval_exchange_transformation.test.cc main.hpp
induction_SOURCES = induction.test.cc main.hpp
induction_benchmark_SOURCES = induction.benchmark.cc main.hpp
rational_linear_subspace_SOURCES = rational_linear_subspace.test.cc main.hpp
cereal_SOURCES = cereal.test.cc main.hpp

# We vendor the header-only library Cereal (serialization with C++ to be able
# to run the tests even when cereal is not installed.
# Cereal is basically not packaged in any distribution since it is a
# header-only library that people always vendor into their source tree.
EXTRA_DIST = external/cereal/include

@VALGRIND_CHECK_RULES@

AM_CPPFLAGS = -I $(srcdir)/../src -I $(builddir)/../src -isystem $(srcdir)/external/cereal/include
AM_LDFLAGS = $(builddir)/../src/libintervalxt.la -lgtest -lbenchmark -lrt -lpthread
# we use gmpxx; its header is using gmp directly so we need to link it in as well
AM_LDFLAGS += -lgmpxx -lgmp
