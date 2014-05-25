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

#ifndef EFC_FUTEX_H_
#define EFC_FUTEX_H_

#include <platform/utils.h>
#include <efc/threads_common.h>


namespace EFC {

class PLATFORM_MAKE_PUBLIC Futex
{
    PLATFORM_MAKE_NONCOPYABLE(Futex)

public:
    class Locker
    {
    public:
        Locker(Futex &futex);
        ~Locker();

        void lock();
        void unlock();

    private:
        bool m_locked;
        Futex &m_futex;
    };

    class Unlocker
    {
    public:
        Unlocker(Locker &lock);
        ~Unlocker();

        void lock();
        void unlock();

    private:
        bool m_locked;
        Locker &m_lock;
    };

public:
    Futex();
    ~Futex();

    void lock();
    void unlock();

private:
    volatile uint32_t m_spin;
#if PLATFORM_OS(UNIX)
    pthread_mutex_t m_mutex;
    pthread_cond_t m_condition;
#elif PLATFORM_OS(WINDOWS)
    CRITICAL_SECTION m_cs;
#endif
};

}

#endif /* EFC_FUTEX_H_ */
