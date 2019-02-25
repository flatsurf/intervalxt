AUTOMAKE_OPTIONS = subdir-objects

# Installed library (libtool (LT) libraries)
lib_LTLIBRARIES = libiet.la

# Installed headers
nobase_include_HEADERS = iet.hh rat_linspace.hh

# Linked-against libraries
libiet_la_LIBADD = -leantic -lgmpxx -lgmp -lm

# sources
libiet_la_SOURCES = src/iet.cpp src/fdiv.cpp

LDADD = libiet.la

check_PROGRAMS = tests/test1 tests/test_irreducible tests/test_induction
tests_test1_SOURCES = tests/test1.cpp
tests_test_irreducible_SOURCES = tests/test_irreducible.cpp
tests_test_induction_SOURCES = tests/test_induction.cpp

TESTS = $(check_PROGRAMS)

