dnl Process this file with autoconf to produce a configure script.

dnl We need at least autoconf 2.69 for this configure.ac to work.
AC_PREREQ([2.69])
AC_INIT([libintervalxt], [3.0.2], [julian.rueth@fsfe.org])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([.])
AM_INIT_AUTOMAKE([1.14.1])

dnl Version number of the library c:r:a.
dnl Set to c+1:0:0 on a non-backwards-compatible change.
dnl Set to c+1:0:a+1 on a backwards-compatible change.
dnl Set to c:r+1:a if the interface is unchanged.
AC_SUBST([libintervalxt_version_info], [4:2:0])

AC_CANONICAL_HOST

dnl Find C++ Compiler
AC_PROG_CXX

dnl Set INSTALL or use install-sh.
AC_PROG_INSTALL

dnl We build our library with libtool.
LT_INIT

dnl We run the valgrind test-suite in make check-valgrind (if valgrind is installed)
AX_VALGRIND_DFLT([sgcheck], [off])
AX_VALGRIND_DFLT([drd], [off])
AX_VALGRIND_DFLT([helgrind], [off])
AX_VALGRIND_CHECK()

dnl Check for required libraries
AC_LANG([C++])

dnl We use some C++17 features, such as if constexpr
AX_CXX_COMPILE_STDCXX(17)

AC_CHECK_HEADERS([boost/type_traits.hpp], , AC_MSG_ERROR([boost headers not found]))

#################
# CHECK FOR FMT #
#################
AC_CHECK_HEADERS([fmt/format.h], , AC_MSG_ERROR([fmt headers not found]))
AX_CXX_CHECK_LIB([fmt], [fmt::v6::getpagesize()], , [
  AX_CXX_CHECK_LIB([fmt], [fmt::v7::getpagesize()], , [AC_MSG_ERROR([fmt library not found])], [-lfmt])
], [-lfmt])

#################
# CHECK FOR GMP #
#################
AC_CHECK_LIB([gmp], [__gmpz_init], , AC_MSG_ERROR([GMP library not found]))
AC_CHECK_HEADERS([gmpxx.h], , AC_MSG_ERROR([GMP headers for C++ not found]))
dnl GMPXX does not contain anything that we can check for with AX_CXX_CHECK_LIB
dnl so we just check for something from the standard library, i.e., that -lgmpxx
dnl goes through
AX_CXX_CHECK_LIB([gmpxx], [std::runtime_error::what () const], , AC_MSG_ERROR([GMP library for C++ not found]))

#################
# CHECK FOR PPL #
#################
AX_CXX_CHECK_LIB([ppl], [Parma_Polyhedra_Library::Constraint_System::print() const], , AC_MSG_ERROR([Parma Polyhedra Library (PPL) not found]))
AC_CHECK_HEADERS([ppl.hh], , AC_MSG_ERROR([Parma Polyhedra Library (PPL) not found]))

#####################
# CHECK FOR E-ANTIC #
#####################
have_eantic=yes
m4_define([AX_WITHOUT_EANTIC], [have_eantic=no; AC_MSG_WARN([$1. This is not required to build libintervalxt but you might not be able to run tests without it or some tests may be skipped, in short make and make install work but make test might throw errors.])])

AC_CHECK_HEADERS([e-antic/renfxx.h], , [AX_WITHOUT_EANTIC([E-ANTIC headers not found])])
AC_CHECK_LIB([eantic], [renf_elem_clear], [ ], [AX_WITHOUT_EANTIC([E-ANTIC library not found])], [-lmpfr])
AX_CXX_CHECK_LIB([eanticxx], [eantic::renf_elem_class], [ ], [AX_WITHOUT_EANTIC([E-ANTIC library not found])], [-lmpfr])

dnl antic is used directly by e-antic in its headers
AC_CHECK_LIB([antic], [nf_elem_sub_si], [ ], [AX_WITHOUT_EANTIC([ANTIC library not found])], [-lmpfr])

dnl We use FLINT's fmpq_sub_si through renfxx.h
AC_CHECK_LIB([flint], [fmpq_sub_si], [ ], [AX_WITHOUT_EANTIC([FLINT library not found])], [-lmpfr])

AC_CHECK_HEADERS([arb.h], , [AX_WITHOUT_EANTIC([Arb headers not found])])
AC_CHECK_LIB([arb], [arb_init], [ ], [AX_WITHOUT_EANTIC([Arb library not found])], [-lmpfr])

AM_CONDITIONAL([HAVE_EANTIC], [test "x$have_eantic" = "xyes"])

########################
# CHECK FOR EXACT-REAL #
########################
have_exactreal=yes
m4_define([AX_WITHOUT_EXACTREAL], [have_exactreal=no; AC_MSG_WARN([$1. This is not required to build libintervalxt but you might not be able to run tests without it or some tests may be skipped, in short make and make install work but make test might throw errors.])])

AC_CHECK_HEADERS([exact-real/element.hpp], , [AX_WITHOUT_EXACTREAL([exact-real headers not found])])
AX_CXX_CHECK_LIB([exactreal], [exactreal::Arb], [ ], [AX_WITHOUT_EXACTREAL([exact-real library not found])])

AM_CONDITIONAL([HAVE_EXACTREAL], [test "x$have_exactreal" = "xyes"])

#######################
# CHECK FOR BENCHMARK #
#######################
AC_ARG_WITH([benchmark], AS_HELP_STRING([--without-benchmark], [Do not build C++ benchmarks that require google/benchmark]))
AS_IF([test "x$with_benchmark" != "xno"],
      [
       with_benchmark=yes
       AC_CHECK_HEADERS([benchmark/benchmark.h], , AC_MSG_ERROR([benchmark headers not found; run --without-benchmark to disable building of benchmark/]))
      ], [])
AM_CONDITIONAL([HAVE_BENCHMARK], [test "x$with_benchmark" = "xyes"])

AC_CONFIG_HEADERS([intervalxt/config.h])
AC_CONFIG_FILES([Makefile src/Makefile test/Makefile benchmark/Makefile])

AC_OUTPUT
