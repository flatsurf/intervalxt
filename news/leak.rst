*+Added:**

* `Component::decomposition` to get a reference back to the `DynamicalDecomposition` of which a `Component` forms a part.

* `Component::topContour(Label)` and `Component::bottomContour(Label)` to
  directly access an element of a contour to replace the removed `HalfEdge`
  constructor.

**Changed:**

* A `DynamicalDecomposition` can now be copied (sharing a reference to the same underlying data.)

* Simplified `noexcept` qualifications on all methods in the public API.

**Removed:**

* Public `HalfEdge` constructor. Half edges should always be created by extracting them from a component's contour.

**Fixed:**

* Fixed two memory leaks in decomposition due to circular sequences of shared
  pointers. And refactored the internal data structures quite heavily.
