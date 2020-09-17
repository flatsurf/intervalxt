**Added:**

* The `Lengths` of an `IntervalExchangeTransformation` are not available as
  `iet.lengths` in Python. This is probably limited to interval exchange
  transformations originally created through the Python interface.

**Changed:**

* In the Python interface, `IET` has been renamed to
  `IntervalExchangeTransformation` and `makeLengths` to `Lengths`.

* `sample::Lengths` over integer types now require `gmpxxll` to be available.

**Fixed:**

* Most labels now print nicely in the Python interface.
