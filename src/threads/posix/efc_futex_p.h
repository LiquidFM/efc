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

#include "../efc_futex.h"
#include "../efc_thread.h"

#include "fast/efc_atomics_p.h"

#include <brolly/assert.h>
#include <errno.h>


namespace EFC {

Futex::Futex() :
    m_spin(0)
{
    if (UNLIKELY(pthread_mutex_init(&m_mutex, 0) != 0))
        ASSERT(!"Can't create mutex!");

    if (UNLIKELY(pthread_cond_init(&m_condition, 0) != 0))
        ASSERT(!"Can't create condition variable!");
}

Futex::~Futex()
{
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_condition);
}

void Futex::lock()
{
    int c;

    /* Spin and try to take lock */
    for (int i = 0; i < SPIN_LOOP_COUNT; ++i)
    {
        c = cmpxchg(&m_spin, 0, 1);

        if (!c)
            return;

        cpu_relax();
    }

    /* The lock is now contended */
    if (c == 1)
        c = xchg(&m_spin, 2);

    pthread_mutex_lock(&m_mutex);
    while (c)
    {
        /* Wait in the kernel */
        pthread_cond_wait(&m_condition, &m_mutex);
        c = xchg(&m_spin, 2);
    }
    pthread_mutex_unlock(&m_mutex);
}

void Futex::unlock()
{
    /* Unlock, and if not contended then exit. */
    if (m_spin == 2)
        m_spin = 0;
    else
        if (xchg(&m_spin, 0) == 1)
            return;

    /* Spin and hope someone takes the lock */
    for (int i = 0; i < 2 * SPIN_LOOP_COUNT; ++i)
    {
        if (m_spin)
        {
            /* Need to set to state 2 because there may be waiters */
            if (cmpxchg(&m_spin, 1, 2))
                return;
        }

        cpu_relax();
    }

    /* We need to wake someone up */
    pthread_mutex_lock(&m_mutex);
    pthread_cond_signal(&m_condition);
    pthread_mutex_unlock(&m_mutex);
}

}
