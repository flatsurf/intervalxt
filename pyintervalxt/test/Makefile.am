TESTS = interval_exchange_transformation.py label.py python-doctest.sh
EXTRA_DIST = $(TESTS)

AM_TESTS_ENVIRONMENT = . $(builddir)/test-env.sh;

interval_exchange_transformation.py: test-env.sh
label.py: test-env.sh
python-doctest.sh: test-env.sh

@VALGRIND_CHECK_RULES@

BUILT_SOURCES = test-env.sh python-doctest.sh
EXTRA_DIST += test-env.sh.in python-doctest.sh.in
CLEANFILES = test-env.sh python-doctest.sh
$(builddir)/test-env.sh: $(srcdir)/test-env.sh.in Makefile
	sed -e 's,[@]abs_srcdir[@],$(abs_srcdir),g' -e 's,[@]abs_builddir[@],$(abs_builddir),g' -e 's,[@]pythondir[@],$(pythondir),g' < $< > $@
$(builddir)/python-doctest.sh: $(srcdir)/python-doctest.sh.in Makefile
	sed -e 's,[@]srcdir[@],$(srcdir),g' < $< > $@
	chmod +x $@
