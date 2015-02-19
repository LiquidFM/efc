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

#ifndef EFC_TASK_H_
#define EFC_TASK_H_

#include <platform/utils.h>
#include <efc/ScopedPointer>


namespace EFC {
class TaskThread;


class PLATFORM_MAKE_PUBLIC Task
{
	PLATFORM_MAKE_NONCOPYABLE(Task)

public:
	typedef ScopedPointer<Task> Holder;

public:
	Task();
	virtual ~Task();

protected:
	friend class TaskThread;
	virtual void run(volatile bool &terminate) = 0;
};

}

#endif /* EFC_TASK_H_ */
