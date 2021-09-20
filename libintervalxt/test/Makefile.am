check_PROGRAMS = rational_linear_subspace label component separatrix connection

if HAVE_EANTIC
if HAVE_EXACTREAL
check_PROGRAMS += interval_exchange_transformation dynamical_decomposition cereal coefficients cppyy
endif
endif

TESTS = $(check_PROGRAMS)

rational_linear_subspace_SOURCES = rational_linear_subspace.test.cc main.cc ../src/rational_linear_subspace.cc
label_SOURCES = label.test.cc main.cc
interval_exchange_transformation_SOURCES = interval_exchange_transformation.test.cc main.cc
dynamical_decomposition_SOURCES = dynamical_decomposition.test.cc main.cc
cereal_SOURCES = cereal.test.cc main.cc
cppyy_SOURCES = cppyy.test.cc main.cc
coefficients_SOURCES = coefficients.test.cc main.cc
component_SOURCES = component.test.cc main.cc
separatrix_SOURCES = separatrix.test.cc main.cc
connection_SOURCES = connection.test.cc main.cc

# We vendor the header-only library Cereal (serialization with C++ to be able
# to run the tests even when cereal is not installed.
# Cereal is basically not packaged in any distribution since it is a
# header-only library that people always vendor into their source tree.
EXTRA_DIST = external/cereal/include
# We vendor the header-only catch2 testing framework
EXTRA_DIST += external/catch2/single_include/catch2

@VALGRIND_CHECK_RULES@

AM_CPPFLAGS = -I $(srcdir)/../ -I $(builddir)/../ -isystem $(srcdir)/external/cereal/include -isystem $(srcdir)/external/catch2/single_include/catch2/

AM_LDFLAGS = $(builddir)/../src/libintervalxt.la

if HAVE_EANTIC
AM_LDFLAGS += -leantic -leanticxx -lflint
endif

if HAVE_EXACTREAL
AM_LDFLAGS += -lexactreal
endif
