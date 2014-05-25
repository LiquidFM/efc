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

#ifndef EFC_FAST_MUTEX_P_H_
#define EFC_FAST_MUTEX_P_H_

#include "efc_linux_futex_p.h"


namespace EFC {

Mutex::Mutex() :
	m_mutex(0)
{}

Mutex::~Mutex()
{}

void Mutex::lock()
{
    int c;

    /* Spin and try to take lock */
    for (int i = 0; i < SPIN_LOOP_COUNT; ++i)
    {
        c = cmpxchg(&m_mutex, 0, 1);

        if (!c)
            return;

        cpu_relax();
    }

    /* The lock is now contended */
    if (c == 1)
        c = xchg(&m_mutex, 2);

    while (c)
    {
        /* Wait in the kernel */
    	sys_futex(&m_mutex, FUTEX_WAIT_PRIVATE, 2, NULL, NULL, 0);
        c = xchg(&m_mutex, 2);
    }
}

bool Mutex::tryLock()
{
	/* Try to take the lock, if is currently unlocked */
	return cmpxchg(&m_mutex, 0, 1) == 0;
}

void Mutex::unlock()
{
    /* Unlock, and if not contended then exit. */
    if (m_mutex == 2)
    	m_mutex = 0;
    else
        if (xchg(&m_mutex, 0) == 1)
            return;

    /* Spin and hope someone takes the lock */
    for (int i = 0; i < 2 * SPIN_LOOP_COUNT; ++i)
    {
        if (m_mutex)
        {
            /* Need to set to state 2 because there may be waiters */
            if (cmpxchg(&m_mutex, 1, 2))
                return;
        }

        cpu_relax();
    }

    /* We need to wake someone up */
    sys_futex(&m_mutex, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, 0);
}

}

#endif /* EFC_FAST_MUTEX_P_H_ */
