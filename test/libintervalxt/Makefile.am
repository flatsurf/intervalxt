check_PROGRAMS = iet induction

TESTS = $(check_PROGRAMS)

iet_SOURCES = iet.test.cc main.hpp
induction_SOURCES = induction.test.cc main.hpp

@VALGRIND_CHECK_RULES@

AM_CPPFLAGS = -I $(srcdir)/../../src/libintervalxt -I $(builddir)/../../src/libintervalxt
AM_LDFLAGS = $(builddir)/../../src/libintervalxt/libintervalxt.la -lgtest -lbenchmark -lrt -lpthread
# we use gmpxx; its header is using gmp directly so we need to link it in as well
AM_LDFLAGS += -lgmpxx -lgmp

# Run tests serially so we can see the output without having to check the log file explicitly.
# AUTOMAKE_OPTIONS = serial-tests
