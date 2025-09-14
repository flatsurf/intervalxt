noinst_PROGRAMS = benchmark

benchmark_SOURCES = main.cc induction.benchmark.cc rational_affine_subspace.benchmark.cc ../src/rational_affine_subspace.cc

AM_CPPFLAGS = -I $(srcdir)/.. -I $(builddir)/..
LDADD = $(builddir)/../src/libintervalxt.la
# Google Benchmark and its dependencies
if OS_LINUX
LDADD += -lbenchmark -lrt -lpthread
else
LDADD += -lbenchmark -lpthread
endif
