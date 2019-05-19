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

#include "intervalxt/label.hpp"

namespace intervalxt {
template <typename Tlen, typename Tmat>
Label<Tlen, Tmat>::Label() {
  i1.twin = &i2;
  i2.twin = &i1;
  i1.prev = i1.next = nullptr;
  i2.prev = i2.next = nullptr;
  i1.lab = this;
  i2.lab = this;
  length = 0;
}
}  // namespace intervalxt

// Explicit instantiations of templates so that code is generated for the linker.
#include <gmpxx.h>

template class intervalxt::Label<unsigned long, unsigned long>;
template class intervalxt::Label<mpz_class, mpz_class>;
template class intervalxt::Label<mpz_class, unsigned long>;
