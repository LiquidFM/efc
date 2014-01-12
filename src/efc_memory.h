/** @file efc_memory.h
 *  @brief TODO: Put description here.
 *
 * TODO: Put description here.
 *
 * @copyright
 * This file is part of the EFC library.
 * @n@n
 * Copyright (C) 2011-2014 Dmitriy Vilkov, <dav.daemon@gmail.com>
 * @n@n
 * the EFC library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * @n@n
 * the EFC library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * @n@n
 * You should have received a copy of the GNU General Public License
 * along with the EFC library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EFC_MEMORY_H_
#define EFC_MEMORY_H_

#include <memory>


namespace EFC {

template <typename _Tp, typename _Dp = ::std::default_delete<_Tp> >
using ScopedPointer = ::std::unique_ptr<_Tp, _Dp>;

template <typename _Tp>
using SharedPointer = ::std::shared_ptr<_Tp>;

}

#endif /* EFC_MEMORY_H_ */
