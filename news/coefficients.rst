**Changed:**

* Split `::intervalxt::sample::Arithmetic` into
  `::intervalxt::sample::FloorDivision` and
  `::intervalxt::sample::Coefficients`. The latter now accepts a vector of
  elements and produces a vector of Q-vectors that correspond to the given
  elements after being brought into the same vector space. This is a breaking
  API change.

* The method `Lengths::coefficients()` now expects a vector of labels and
  produces a vector of Q-vectors corresponding to realizations of the
  corresponding lengths in a common Q vector space.

* The method `IntervalExchangeTransformation::safInvariant()` now returns a
  `vector<mpq_class>` instead of a `valarray<mpq_class>`. The valarray had
  always been a source of issues since it does not play nicely with vectors
  that we use everywhere else in the flatsurf stack.
