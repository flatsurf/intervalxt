all-local:
	mkdir -p $(builddir)/build
	cd $(srcdir) && $(PYTHON) $(abs_srcdir)/setup.py build --verbose --build-base $(abs_builddir)/build

install-exec-local:
	$(PYTHON) $(abs_srcdir)/setup.py install --prefix $(DESTDIR)$(prefix) --single-version-externally-managed --record $(DESTDIR)$(pythondir)/pyintervalxt/install_files.txt --verbose

uninstall-local:
	cat $(DESTDIR)$(pythondir)/pyintervalxt/install_files.txt | xargs rm -rf
	rm -rf $(DESTDIR)$(pythondir)/pyintervalxt

clean-local:
	-rm -rf pyintervalxt/__pycache__ pyintervalxt.egg-info build .pytest_cache

EXTRA_DIST = setup.py
