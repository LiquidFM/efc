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
#include "fast/efc_fast_waitcondition_p.h"
#else
#include <brolly/assert.h>
#include <sys/time.h>
#include <errno.h>


namespace EFC {

WaitCondition::WaitCondition()
{
    if (UNLIKELY(::pthread_cond_init(&m_condition, 0) != 0))
    	ASSERT(!"Can't create condition variable!");
}

WaitCondition::~WaitCondition()
{
    if (UNLIKELY(::pthread_cond_destroy(&m_condition) != 0))
        ASSERT(!"Can't destroy condition variable!");
}

void WaitCondition::wakeOne()
{
    if (UNLIKELY(::pthread_cond_signal(&m_condition) != 0))
        ASSERT(!"Can't signal condition variable!");
}

void WaitCondition::wakeAll()
{
    if (UNLIKELY(::pthread_cond_broadcast(&m_condition) != 0))
        ASSERT(!"Can't signal to condition variable!");
}

void WaitCondition::wait(Mutex &mutex)
{
    if (UNLIKELY(::pthread_cond_wait(&m_condition, &mutex.m_mutex) != 0))
        ASSERT(!"Can't wait on condition variable!");
}

bool WaitCondition::wait(Mutex &mutex, unsigned msec)
{
	struct timeval tv = {0, 0};

	if (UNLIKELY(::gettimeofday(&tv, NULL) != 0))
        ASSERT(!"Can't get system time!");
	else
	{
		unsigned sec = msec / 1000;
		struct timespec ts = { tv.tv_sec + sec, (tv.tv_usec + (msec - sec * 1000) * 1000) * 1000 };
		int res = ::pthread_cond_timedwait(&m_condition, &mutex.m_mutex, &ts);

		if (res == 0)
			return true;
		else
			if (UNLIKELY(res != ETIMEDOUT))
				ASSERT(!"Can't wait on condition variable!");
	}

	return false;
}

}

#endif
