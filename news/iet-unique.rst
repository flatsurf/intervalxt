**Fixed:**

* `Component::iet()` returned a `unique_ptr`; there's no need for that since
  `IntervalExchangeTransformation` can be moved (and easily copied if needed.)
  So we now return an IntervalExchangeTransformation instead. (technically, a
  breaking API & ABI change but nobody is using this functionality that we
  released a few minutes ago yet.)
