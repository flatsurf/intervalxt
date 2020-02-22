/* ********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019 Julian Rüth
 *
 *  intervalxt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  intervalxt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with intervalxt. If not, see <https://www.gnu.org/licenses/>.
 * *******************************************************************/

#ifndef LIBINTERVALXT_CEREAL_HPP
#define LIBINTERVALXT_CEREAL_HPP

#include <map>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/push_back.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include <fmt/format.h>

#include "interval_exchange_transformation.hpp"
#include "label.hpp"
#include "length.hpp"

namespace intervalxt {

template <typename T>
struct Serialization {
  template <typename Archive>
  auto save(Archive&, const T&) {
    static_assert(!std::is_same_v<T, T>, "no Serialization specialization for this type defined");
  }

  template <typename Archive>
  void load(Archive&, T&) {
    static_assert(!std::is_same_v<T, T>, "no Serialization specialization for this type defined");
  }
};

template <>
struct Serialization<Label> {
  template <typename Archive>
  size_t save(Archive&, const Label& self) {
    return self.id;
  }

  template <typename Archive>
  void load(const Archive&, Label& self, const size_t& id) {
    self.id = id;
  }
};

template <>
struct Serialization<IntervalExchangeTransformation> {
  template <typename Archive>
  void save(Archive& archive, const IntervalExchangeTransformation& self) {
    archive(cereal::make_nvp("top", self.top()));
    archive(cereal::make_nvp("bottom", self.bottom()));
    archive(cereal::make_nvp("lengths", self.lengths()));
  }

  template <typename Archive>
  void load(Archive& archive, IntervalExchangeTransformation& self) {
    std::vector<Label> top, bottom;
    std::shared_ptr<Lengths> lengths;

    archive(cereal::make_nvp("top", top));
    archive(cereal::make_nvp("bottom", bottom));
    archive(cereal::make_nvp("lengths", lengths));

    if (lengths == nullptr) {
      assert(top.empty() && bottom.empty() && "There must not be top & bottom labels when there are no lengths to give meaning to them.");
      // The default initialized self is already the correct deserialization.
      return;
    }

    self = IntervalExchangeTransformation(lengths, top, bottom);
  }
};

/*
template <>
struct Serialization<Lengths> {
  struct ErasedSerializationHelperBase {
    virtual ~ErasedSerializationHelperBase(){} 

    virtual Lengths lengths() = 0;

    template <typename Archive>
    void serialize(Archive& archive) {}
  };

  template <typename T>
  struct ErasedSerializationHelper : public ErasedSerializationHelperBase {
    template <typename Archive>
    void serialize(Archive& archive) { archive(*t); }

    Lengths lengths() override { return *t; }

    T* t = new T();
  };

};

// Note: For some strange reason, we cannot use a load/save pair here. Doing
// so makes cereal complain that Label (!) has more than one save() function.
template <typename Archive>
void serialize(Archive& archive, Lengths& self) {
  if constexpr (Archive::is_loading::value) {
    static_assert(!Archive::is_saving::value, "serialize() must be either loading or saving but this archive is doing both");

    std::unique_ptr<Serialization<Lengths>::ErasedSerializationHelperBase> serializable;

    archive(serializable);

    self = serializable->lengths();

    // throw std::logic_error(fmt::format("not implemented for {}", typeid_of(self).name()));
  } else {
    static_assert(Archive::is_saving::value, "serialize() must be either loading or saving but this archive is not doing either");

    std::unique_ptr<Serialization<Lengths>::ErasedSerializationHelperBase> serializable(static_cast<Serialization<Lengths>::ErasedSerializationHelperBase*>(self.serializable()));

    archive(serializable);
  }
}

#define LIBINTERVALXT_REGISTER_SERIALIZABLE_LENGTHS(TYPE) \
  CEREAL_REGISTER_TYPE(::intervalxt::Serialization<::intervalxt::Lengths>::ErasedSerializationHelper<TYPE>); \
  CEREAL_REGISTER_POLYMORPHIC_RELATION(::intervalxt::Serialization<::intervalxt::Lengths>::ErasedSerializationHelperBase, ::intervalxt::Serialization<::intervalxt::Lengths>::ErasedSerializationHelper<TYPE>);
*/

// Since Lengths is type-erased, its serialization works differently. If you
// don't want to support serialization of your Lengths, then there is nothing
// to do. Otherwise, implement any cereal serialization scheme, e.g., by
// defining a serialize method in your Lengths class:
// ```
// template <typename Archive>
// void serialize(Archive&);
// ```
// Additionally, you must register your lengths **before** including this file.
// It is probably a good pattern to put this registration together with the
// implementation of serialize into a cereal.hpp file that users that want to
// use cereal serialization can optionally include:
// ```
// #include <cereal/cereal.hpp>
// #include <intervalxt/lengths.hpp>
//
// template <typename Archive>
// void MyLengths::serialize(Archive& archive) { ... }
//
// LIBINTERVALXT_LENGTHS_REGISTER_SERIALIZATION("ThisStringWillShowUpInSerializationOutput", (MyLengths));
// ```

      /*
template <typename Archive>
void save(Archive& archive, const Lengths& self) {
  BOOST_PP_SEQ_FOR_EACH(LIBINTERVALXT_TRY_SAVE, _, LIBINTERVALXT_LENGTHS_REGISTERED_SERIALIZATION);
  throw std::logic_error("not implemented: this Lengths do not support serialization; did you forget to call LIBINTERVALXT_LENGTHS_REGISTER_SERIALIZATION?");
}
*/

/*
template <typename Archive, typename T>
void save(Archive& archive, const Length<T>& self) {
  archive(cereal::make_nvp("value", self.value));
}

template <typename Archive, typename T>
void load(Archive& archive, Length<T>& self) {
  T value;
  archive(value);
  self = value;
}

template <typename Archive, typename Length>
void save(Archive& archive, const Label<Length>& self) {
  auto uuid = boost::lexical_cast<std::string>(static_cast<boost::uuids::uuid>(*self.impl->id));
  archive(cereal::make_nvp("uuid", uuid));
  archive(cereal::make_nvp("length", self.impl->length));
}

template <typename Archive, typename Length>
void load(Archive& archive, Label<Length>& self) {
  std::string uuid;
  archive(cereal::make_nvp("uuid", uuid));
  self.impl->id = detail::Id::make(boost::lexical_cast<boost::uuids::uuid>(uuid));
  archive(cereal::make_nvp("length", self.impl->length));
}

template <typename Archive, typename Length>
void save(Archive& archive, const IntervalExchangeTransformation<Length>& self) {
  archive(cereal::make_nvp("top", self.top()));
  archive(cereal::make_nvp("bottom", self.bottom()));
}

template <typename Archive, typename Length>
void load(Archive& archive, IntervalExchangeTransformation<Length>& self) {
  std::vector<Label<Length>> top, bottom;
  archive(cereal::make_nvp("top", top));
  archive(cereal::make_nvp("bottom", bottom));
  self = IntervalExchangeTransformation<Length>(top, bottom);
}
*/

}  // namespace intervalxt

#endif
