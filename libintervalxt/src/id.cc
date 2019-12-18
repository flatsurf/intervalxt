/**********************************************************************
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
 *********************************************************************/

#include <boost/uuid/random_generator.hpp>

#include "../intervalxt/detail/id.hpp"

#include "external/unique-factory/unique_factory.hpp"

namespace intervalxt {
namespace detail {

namespace {
auto& generator() {
  static boost::uuids::random_generator generator;
  return generator;
}

auto& id2uuid() {
  static UniqueFactory<boost::uuids::uuid, std::weak_ptr<const Id>> factory;
  return factory;
}

auto& uuid2id() {
  static UniqueFactory<const Id, boost::uuids::uuid> factory;
  return factory;
}

auto& id2name() {
  static UniqueFactory<std::string, std::weak_ptr<const Id>> factory;
  return factory;
}

}  // namespace

Id::Id() noexcept {}

Id::operator boost::uuids::uuid() const noexcept {
  // Note that this is thread-safe since the factories are thread-safe.
  auto ret = id2uuid().get(this->weak_from_this(), [&]() {
    // It is safe to call generator() here even though it is not thread-safe.
    // Since the factory allows only one get() to run at a time.
    return new boost::uuids::uuid(generator()());
  });
  // Entering the uuid as a key makes sure that it's being kept alive in the
  // id2uuid cache as well.
  uuid2id().get(*ret, [&]() {
    return this->shared_from_this();
  });
  return *ret;
}

std::ostream& operator<<(std::ostream& os, const Id& self) {
  // Note that this is thread-safe since the factories are thread-safe.
  auto ret = *id2name().get(self.weak_from_this(), [&]() {
    // It is safe to call generator() here even though it is not thread-safe.
    // Since the factory allows only one get() to run at a time.
    static int next = 0;
    std::string ret;
    int current = next++;
    while (current || ret.size() == 0) {
      int offset = current % (2 * 26);
      if (offset < 26) {
        ret += static_cast<char>('a' + offset);
      } else {
        ret += static_cast<char>('A' + (offset - 26));
      }
      current /= (2 * 26);
    }
    return new std::string(ret);
  });
  return os << ret;
}

std::shared_ptr<const Id> Id::make() noexcept {
  return std::shared_ptr<Id>(new Id());
}

std::shared_ptr<const Id> Id::make(const boost::uuids::uuid& uuid) noexcept {
  auto id = uuid2id().get(uuid, [&]() {
    return new Id();
  });
  id2uuid().get(id->weak_from_this(), [&]() {
    return new boost::uuids::uuid(uuid);
  });
  return id;
}

}  // namespace detail
}  // namespace intervalxt
