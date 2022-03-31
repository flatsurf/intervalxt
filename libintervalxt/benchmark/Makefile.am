noinst_PROGRAMS = benchmark

benchmark_SOURCES = main.cc induction.benchmark.cc rational_affine_subspace.benchmark.cc ../src/rational_affine_subspace.cc

AM_CPPFLAGS = -I $(srcdir)/.. -I $(builddir)/..
AM_LDFLAGS = $(builddir)/../src/libintervalxt.la
# Google Benchmark and its dependencies
AM_LDFLAGS += -lbenchmark -lrt -lpthread

