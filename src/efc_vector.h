/* This file is part of the EFC library.
 *
 * Copyright (C) 2013 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * the EFC library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * the EFC library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the EFC library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EFC_VECTOR_H_
#define EFC_VECTOR_H_

#include <vector>
#include <cstddef>
#include <platform/utils.h>
#include <efc/type_traits>


namespace EFC {

template <typename Value>
using Vector = ::std::vector<Value>;

}

#endif /* EFC_VECTOR_H_ */
