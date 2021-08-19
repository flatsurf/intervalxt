**Added:**

* Added `LIBINTERVALXT_API` markers to support building with `-fvisibility=hidden -fvisibility-inlines-hidden`.
* Added support for version script when linking on Linux, i.e., producing a library that only exports a minimal set of symbols. Enable with ./configure --with-version-script.
