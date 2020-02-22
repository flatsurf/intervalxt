TESTS = interval_exchange_transformation.py
EXTRA_DIST = $(TESTS)

AM_TESTS_ENVIRONMENT = . $(builddir)/test-env.sh;

interval_exchange_transformation.py: test-env.sh

@VALGRIND_CHECK_RULES@

BUILT_SOURCES = test-env.sh
EXTRA_DIST += test-env.sh.in
CLEANFILES = test-env.sh
$(builddir)/test-env.sh: $(srcdir)/test-env.sh.in Makefile
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' -e 's,[@]abs_builddir[@],$(abs_builddir),g' -e 's,[@]pythondir[@],$(pythondir),g' < $< > $@
