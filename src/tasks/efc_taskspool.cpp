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

#include "efc_taskspool.h"
#include "efc_taskthread.h"

#include <efc/ScopedPointer>


namespace EFC {

/* TODO(Vilkov): We should use a static allocator for threads lists. */
WARN_UNUSED_RETURN_OFF

TasksPool::TasksPool(int maxThreads) :
	m_maxThreads(maxThreads)
{}

TasksPool::~TasksPool()
{
	clear();

	for (Threads::iterator i = m_threads.begin(), end = m_threads.end(); i != end; i = m_threads.erase(i))
		delete (*i);
}

bool TasksPool::haveFreeThreads() const
{
    Futex::Locker lock(m_futex);
    return !m_freeThreads.empty() || m_threads.size() < m_maxThreads;
}

void TasksPool::handleImmediately(Task::Holder &task)
{
	Futex::Locker lock(m_futex);

	if (!m_freeThreads.empty())
	{
        m_busyThreads[task.get()] = m_freeThreads.front();
        m_freeThreads.front()->handle(task.release());
        m_freeThreads.pop_front();
    }
	else
	{
		if (m_threads.size() < m_maxThreads)
		{
		    bool valid = true;
		    ScopedPointer<TaskThread> thread(new (std::nothrow) TaskThread(this, task.get(), valid));

            if (LIKELY(thread != NULL))
                if (LIKELY(valid == true))
                {
                    m_busyThreads[task.get()] = thread.get();
                    m_threads.push_back(thread.get());
                    thread.release();
                    task.release();
                    return;
                }
        }

		m_tasks.push_front(task.get());
		task.release();
	}
}

void TasksPool::handle(Task::Holder &task)
{
	Futex::Locker lock(m_futex);

	if (!m_freeThreads.empty())
	{
	    m_busyThreads[task.get()] = m_freeThreads.front();
		m_freeThreads.front()->handle(task.release());
		m_freeThreads.pop_front();
	}
	else
	{
		if (m_threads.size() < m_maxThreads)
		{
            bool valid = true;
			ScopedPointer<TaskThread> thread(new (std::nothrow) TaskThread(this, task.get(), valid));

			if (LIKELY(thread != NULL))
				if (LIKELY(valid == true))
				{
			        m_busyThreads[task.get()] = thread.get();
					m_threads.push_back(thread.get());
					thread.release();
					task.release();
					return;
				}
		}

		m_tasks.push_back(task.get());
		task.release();
	}
}

void TasksPool::cancel(const Task *task)
{
    Futex::Locker lock(m_futex);
    BusyThreads::const_iterator i = m_busyThreads.find(task);

    if (i != m_busyThreads.end())
        i->second->cancel();
}

void TasksPool::clear()
{
	Futex::Locker lock(m_futex);

	for (Tasks::iterator i = m_tasks.begin(), end = m_tasks.end(); i != end; i = m_tasks.erase(i))
		delete (*i);
}

void TasksPool::nextTask(TaskThread *thread, Task *&task)
{
	Futex::Locker lock(m_futex);
	m_busyThreads.erase(task);

	if (m_tasks.empty())
	{
	    task = NULL;
		m_freeThreads.push_back(thread);
	}
	else
	{
        m_busyThreads[task = m_tasks.front()] = thread;
		m_tasks.pop_front();
	}
}

WARN_UNUSED_RETURN_ON
}
