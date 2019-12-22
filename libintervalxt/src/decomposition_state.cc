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

#include <ostream>

#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/to_container.hpp>

#include "../intervalxt/label.hpp"
#include "../intervalxt/connection.hpp"

#include "impl/decomposition_state.hpp"

namespace intervalxt {

using std::ostream;
using boost::algorithm::join;
using ranges::view::transform;
using ranges::view::filter;
using ranges::to;

ostream& operator<<(ostream& os, const DecompositionState& self) {
  const auto& render = [&](const auto& state) {
    return join(state
      | filter([](const auto& connections) { return not connections.second.left.empty() || not connections.second.right.empty(); })
      | transform([](const auto& connections) {
        return boost::lexical_cast<std::string>(std::hash<Label>()(connections.first))
          + ": left=["
          + join(connections.second.left | transform([](const auto& connection) { return boost::lexical_cast<std::string>(connection); }), ", ")
          + "], right=["
          + join(connections.second.right | transform([](const auto& connection) { return boost::lexical_cast<std::string>(connection); }), ", ")
          + "]"; 
        })
      | to<vector<std::string>> ,
    ", ");
  };

  return os << "DecompositionState(top=" << render(self.top) << ", bottom=" << render(self.bottom) << ")";
}

}
