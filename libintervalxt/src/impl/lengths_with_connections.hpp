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

#ifndef LIBINTERVALXT_LENGTHS_WITH_CONNECTIONS_HPP
#define LIBINTERVALXT_LENGTHS_WITH_CONNECTIONS_HPP

#include <memory>
#include <vector>
#include <string>

#include <gmpxx.h>

#include "forward.hpp"
#include "../../intervalxt/label.hpp"
#include "../../intervalxt/lengths.hpp"

namespace intervalxt {

class LengthsWithConnections {
 public:
  LengthsWithConnections(std::shared_ptr<Lengths>, std::shared_ptr<DecompositionState>);
 
  void push(Label);
  void pop();
  void subtract(Label);
  Label subtractRepeated(Label);
  std::vector<mpq_class> coefficients(Label) const;
  int cmp(Label) const;
  int cmp(Label, Label) const;
  Length get(Label) const;
  std::string render(Label) const;

 private:
  void subtract(Label minuend, Label subtrahend);

  const std::shared_ptr<Lengths> lengths;
  const std::shared_ptr<DecompositionState> decomposition;

  std::vector<Label> stack;
};

}

#endif



