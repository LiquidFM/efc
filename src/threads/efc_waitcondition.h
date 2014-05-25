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

#ifndef EFC_WAITCONDITION_H_
#define EFC_WAITCONDITION_H_

#include <efc/Mutex>


namespace EFC {

class PLATFORM_MAKE_PUBLIC WaitCondition
{
    PLATFORM_MAKE_NONCOPYABLE(WaitCondition)

public:
    WaitCondition();
    ~WaitCondition();

    void wakeOne();
    void wakeAll();
    void wait(Mutex &mutex);
    bool wait(Mutex &mutex, unsigned msec);

private:
    condition_t m_condition;
};

}

#endif /* EFC_WAITCONDITION_H_ */
