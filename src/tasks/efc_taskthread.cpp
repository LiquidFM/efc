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

#include "efc_taskthread.h"
#include "efc_taskspool.h"

#include <brolly/assert.h>


namespace EFC {

TaskThread::TaskThread(TasksPool *pool, Task::Holder &task) :
	Thread(),
    m_vaild(true),
    m_abort(false),
	m_pool(pool)
{
	ASSERT(task != NULL);
	ASSERT(m_pool != NULL);

	Mutex::Locker lock(m_handler.mutex());

	if (LIKELY(start() == NoError))
	{
		m_task = std::move(task);
		m_handler.waitForStart();
	}
	else
		m_vaild = false;
}

TaskThread::~TaskThread()
{
	Mutex::Locker lock(m_handler.mutex());

	if (m_handler.isRunning())
	{
		m_abort = true;
		m_handler.waitForStop();
	}
}

void TaskThread::handle(Task::Holder &task)
{
	Mutex::Locker lock(m_handler.mutex());
	m_task = std::move(task);
    m_condition.wakeOne();
}

void TaskThread::run()
{
	Mutex::Locker lock(m_handler.mutex());
	m_handler.signalStarted();

    for (;;)
		if (m_task.get() == NULL)
		{
			m_condition.wait(m_handler.mutex());

			if (m_abort)
	            break;
		}
		else
		{
			ScopedPointer<Task> task(std::move(m_task));

			PLATFORM_TRY
			{
				Mutex::Unlocker unlock(lock);
				task->run(m_abort);
			}
			PLATFORM_CATCH(...)
			{}

			if (m_abort)
				break;
			else
				m_pool->nextTask(this, m_task);
		}

	m_handler.signalStopped();
}

}
