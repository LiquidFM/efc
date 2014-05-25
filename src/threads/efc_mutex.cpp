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

#include "efc_mutex.h"

#include <platform/platform.h>
#if PLATFORM_OS(UNIX)
#	include "posix/efc_mutex_p.h"
#elif PLATFORM_OS(WINDOWS)
#	include "win32/efc_mutex_p.h"
#endif


namespace EFC {

Mutex::Locker::Locker(Mutex &mutex) :
	m_locked(true),
	m_mutex(mutex)
{
    m_mutex.lock();
}

Mutex::Locker::~Locker()
{
    if (m_locked)
        m_mutex.unlock();
}

void Mutex::Locker::lock()
{
    m_mutex.lock();
    m_locked = true;
}

void Mutex::Locker::unlock()
{
    m_mutex.unlock();
    m_locked = false;
}

Mutex::Unlocker::Unlocker(Locker &lock) :
	m_locked(false),
	m_lock(lock)
{
    m_lock.unlock();
}

Mutex::Unlocker::~Unlocker()
{
    if (!m_locked)
        m_lock.lock();
}

void Mutex::Unlocker::lock()
{
    m_lock.lock();
    m_locked = true;
}

void Mutex::Unlocker::unlock()
{
    m_lock.unlock();
    m_locked = false;
}

}
