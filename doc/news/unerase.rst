**Added:**

* Added fallback printing of `interalxt::Label` to an `std::ostream` when `Lengths::render` is not available.

* Added fallback printing of a `Label` in Python when the corresponding `Lengths` are not available.

**Removed:**

* Removed unused `length.ipp` header.

**Fixed:**

* Made all methods of `intervalxt::Length` and `intervalxt::Length` accessible from Python.

* Removed libtool scripts from repository.

* Removed broken binder tests from CI. (We should instead build a conda package and run tests with that package installed.)
