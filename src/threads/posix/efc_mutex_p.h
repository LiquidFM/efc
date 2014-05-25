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

#if PLATFORM_USE(FAST_MUTEXES)
#include "fast/efc_fast_mutex_p.h"
#else

#include <brolly/assert.h>
#include <string.h>
#include <errno.h>


namespace EFC {

Mutex::Mutex()
{
	pthread_mutexattr_t attr;

	::memset(&attr, 0, sizeof(pthread_mutexattr_t));

#if PLATFORM_ENABLE(DEADLOCKS_DETECTION)
	if (UNLIKELY(::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK) != 0))
        ASSERT(!"Can't set mutex attributes!");
#else
	if (UNLIKELY(::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_DEFAULT) != 0))
        ASSERT(!"Can't set mutex attributes!");
#endif

	if (UNLIKELY(::pthread_mutex_init(&m_mutex, &attr) != 0))
        ASSERT(!"Can't create mutex!");
}

Mutex::~Mutex()
{
    if (UNLIKELY(::pthread_mutex_destroy(&m_mutex) != 0))
        ASSERT(!"Can't destroy mutex!");
}

void Mutex::lock()
{
#if PLATFORM_ENABLE(DEADLOCKS_DETECTION)
	int res = ::pthread_mutex_lock(&m_mutex);

	if (UNLIKELY(res != 0))
		if (LIKELY(res == EDEADLK))
			ASSERT(!"Deadlock detected!");
		else
			ASSERT(!"Can't lock mutex!");
#else
	if (UNLIKELY(::pthread_mutex_lock(&m_mutex) != 0))
		ASSERT(!"Can't lock mutex!");
#endif
}

bool Mutex::tryLock()
{
    int res = ::pthread_mutex_trylock(&m_mutex);

    if (res == 0)
        return true;
    else
		if (LIKELY(res == EBUSY))
			return false;
		else
		{
#if PLATFORM_ENABLE(DEADLOCKS_DETECTION)
			if (LIKELY(res == EDEADLK))
				ASSERT(!"Deadlock detected!");
#endif
	        ASSERT(!"Can't tryLock mutex!");
			return false;
		}
}

void Mutex::unlock()
{
    if (UNLIKELY(::pthread_mutex_unlock(&m_mutex) != 0))
        ASSERT(!"Can't unlock mutex!");
}

}

#endif
