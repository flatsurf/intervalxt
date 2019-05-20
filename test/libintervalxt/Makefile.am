check_PROGRAMS = rational_linear_subspace interval_exchange_transformation induction induction_benchmark

TESTS = $(check_PROGRAMS)

interval_exchange_transformation_SOURCES = interval_exchange_transformation.test.cc main.hpp
induction_SOURCES = induction.test.cc main.hpp
induction_benchmark_SOURCES = induction.benchmark.cc main.hpp
rational_linear_subspace_SOURCES = rational_linear_subspace.test.cc main.hpp

@VALGRIND_CHECK_RULES@

AM_CPPFLAGS = -I $(srcdir)/../../src/libintervalxt -I $(builddir)/../../src/libintervalxt
AM_LDFLAGS = $(builddir)/../../src/libintervalxt/libintervalxt.la -lgtest -lbenchmark -lrt -lpthread
# we use gmpxx; its header is using gmp directly so we need to link it in as well
AM_LDFLAGS += -lgmpxx -lgmp

# Run tests serially so we can see the output without having to check the log file explicitly.
# AUTOMAKE_OPTIONS = serial-tests
