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

#include <stddef.h>
#include <string.h>
#include <errno.h>


namespace EFC {

Mutex::Mutex() :
	m_mutex(::CreateMutex(NULL, false, NULL))
{
	if (UNLIKELY(m_mutex == NULL))
        ASSERT(!"Can't create mutex!");
}

Mutex::~Mutex()
{
    if (UNLIKELY(::CloseHandle(m_mutex) == 0))
        ASSERT(!"Can't destroy mutex!");
}

void Mutex::lock()
{
	if (UNLIKELY(::WaitForSingleObject(m_mutex, INFINITE) != WAIT_OBJECT_0))
		ASSERT(!"Can't lock mutex!");
}

bool Mutex::tryLock()
{
	DWORD res = ::WaitForSingleObject(m_mutex, 1);

    if (res == WAIT_OBJECT_0)
        return true;
    else
		if (LIKELY(res == WAIT_TIMEOUT))
			return false;
		else
		{
	        ASSERT(!"Can't tryLock mutex!");
			return false;
		}
}

void Mutex::unlock()
{
	if (UNLIKELY(::ReleaseMutex(m_mutex) == 0))
        ASSERT(!"Can't unlock mutex!");
}

}
