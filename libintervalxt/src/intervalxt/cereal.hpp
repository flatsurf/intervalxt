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

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

#include "intervalxt/interval_exchange_transformation.hpp"
#include "intervalxt/label.hpp"
#include "intervalxt/length.hpp"

namespace intervalxt {

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

}  // namespace intervalxt
