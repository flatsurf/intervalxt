check_PROGRAMS = rational_affine_subspace.test label.test component.test separatrix.test connection.test

if HAVE_EANTIC
if HAVE_EXACTREAL
check_PROGRAMS += interval_exchange_transformation.test dynamical_decomposition.test cereal.test coefficients.test cppyy.test
endif
endif

TESTS = $(check_PROGRAMS)

rational_affine_subspace_test_SOURCES = rational_affine_subspace.test.cc main.cc ../src/rational_affine_subspace.cc
label_test_SOURCES = label.test.cc main.cc
interval_exchange_transformation_test_SOURCES = interval_exchange_transformation.test.cc main.cc
dynamical_decomposition_test_SOURCES = dynamical_decomposition.test.cc main.cc
cereal_test_SOURCES = cereal.test.cc main.cc
cppyy_test_SOURCES = cppyy.test.cc main.cc
coefficients_test_SOURCES = coefficients.test.cc main.cc
component_test_SOURCES = component.test.cc main.cc
separatrix_test_SOURCES = separatrix.test.cc main.cc
connection_test_SOURCES = connection.test.cc main.cc

# We vendor the header-only library Cereal (serialization with C++ to be able
# to run the tests even when cereal is not installed.
# Cereal is basically not packaged in any distribution since it is a
# header-only library that people always vendor into their source tree.
EXTRA_DIST = external/cereal/include
# We vendor the header-only catch2 testing framework
EXTRA_DIST += external/catch2/single_include/catch2

@VALGRIND_CHECK_RULES@

AM_CPPFLAGS = -I $(srcdir)/../ -I $(builddir)/../ -isystem $(srcdir)/external/cereal/include -isystem $(srcdir)/external/catch2/single_include/catch2/

LDADD = ../src/libintervalxt.la

if HAVE_EANTIC
LDADD += -leantic -leanticxx -lflint
endif

if HAVE_EXACTREAL
LDADD += -lexactreal -lflint
endif
