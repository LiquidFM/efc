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

WaitEvent::WaitEvent(bool initialState, bool autoReset) :
	m_autoReset(autoReset), m_state(initialState)
{}

void WaitEvent::wait()
{
    Mutex::Locker lock(m_mutex);
    while(!m_state)
        m_condition.wait(m_mutex);
}

bool wait(unsigned msec)
{
    ASSERT(!"Not implemeted!");
    return false;
}

void WaitEvent::set()
{
    Mutex::Locker lock(m_mutex);
    m_state = true;
    m_condition.wakeAll();
}

void WaitEvent::reset()
{
    Mutex::Locker lock(m_mutex);
    m_state = false;
    m_condition.wakeAll();
}

}
