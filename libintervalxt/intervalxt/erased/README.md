Cereal Serialization Support for Type Erased Types
==================================================

The steps below let you use type erasure with [cereal](http://uscilab.github.io/cereal/quickstart.html) to serialize types
such as [`boost::type_erasure::any`](https://www.boost.org/doc/libs/1_72_0/doc/html/boost_typeerasure.html).

Note that there are some assumptions for this to work:

* You need to be able to adapt the type erasure definition [^1]
* The any you want to serialize capture by value and [not by reference](https://www.boost.org/doc/libs/1_70_0/doc/html/boost_typeerasure/any.html#boost_typeerasure.any.references) [^2]
* The serializable unerased types must be default constructible and copy-assignable [^3]

Make a Boost Type Erasure Type Serializable
-------------------------------------------

For these notes, we'll assume that your type's definition is

```cpp
using Interface = boost::mpl::vector<
  …
>;

using ErasedType = boost::type_erasure::any<Interface>;
```

1. Add a free function to your type erasure declaration that converts to a serializable polymorphic type.

```cpp
#include <intervalxt/erased/boost.hpp>

// Forward declare a traits class that connects an erased serializable type
// with your ErasedType.
struct ErasedTypeSerialization;

using Interface = boost::mpl::vector<
  …,
  ::intervalxt::erased::is_serializable<ErasedTypeSerialization>
>

using ErasedType = …;

// Implement the traits class from above.
struct ErasedTypeSerialization {
  using Erased = ErasedType;
};
```

2. Implement a free function `serializable()`

You might want to put this definition right below your `using ErasedType = …;`.

```cpp
#include <intervalxt/erased/serializable.hpp>

template <typename T>
::intervalxt::erased::Serializable<ErasedTypeSerialization>::SerializableWrap<T> serializable(const T& unerased) {
  return ::intervalxt::erased::Serializable<ErasedTypeSerialization>::make(unerased);
}
```

3. Make your unerased types serializable

You might want to do this in a header that your users are not required to pull
in, say `cereal.hpp`, so that they do not have to pull in the entire cereal
stack unless they are actually using serialization.

```cpp
#include <cereal/types/polymorphic.hpp>
#include <intervalxt/erased/cereal.hpp>

// Implement serialization using any of the methods supported by cereal, e.g.,
// with a free serialize() function.
template <typename Archive>
void serialize(Archive& archive, Unerased& unerased) {
  …
}

// Register the Serializable<> types with cereal's polymorphism machinery for
// each unerased type you want to deserialize.
// IMPORTANT: Tell your users that they must include this header file *after*
// any cereal/archives/ files.
LIBINTERVALXT_ERASED_REGISTER((::full::namespace::ErasedTypeSerialization), (::full::namespace::Unerased));
```

That's it. Now your erased types should serialize and deserialize (they
serialize as unique pointers to a helper class that contains your unerased
type.)

[^1]: Alternatively, we could have used `dynamic_any_cast` to cross cast the
  erased type to a type that has the `serializable()` introduced in 1. The
  downside of this approach is that all unerased types would have to state that
  they satisfy this requirement explicitly by calling `register_binding`. Since
  this is less intrusive, it would still have been the better choice. Pull
  Requests are welcome.

[^2]: It is a unclear to the authors what is supposed to happen if something is
  captured by reference. Cereal does not support references so we do not
  either.

[^3]: Both of these do not seem to be hard requirements. Pull Requests to
  remove these requirements are welcome. Cereal supports serialization of types
  without a default constructor, and erased types can be [constructed inplace
  in boost](https://www.boost.org/doc/libs/1_70_0/doc/html/boost_typeerasure/any.html#boost_typeerasure.any.construction).
