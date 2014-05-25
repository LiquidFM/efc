/**
 * This file is part of the EFC library.
 *
 * Copyright (C) 2011-2014 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

namespace EFC {

Futex::Futex()
{
    ::InitializeCriticalSection(&m_cs);
}

Futex::~Futex()
{
    ::DeleteCriticalSection(&m_cs);
}

void Futex::lock()
{
    ::EnterCriticalSection(&m_cs);
}

void Futex::unlock()
{
    ::LeaveCriticalSection(&m_cs);
}

}
