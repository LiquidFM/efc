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

#ifndef EFC_TASKSPOOL_H_
#define EFC_TASKSPOOL_H_

#include <efc/Map>
#include <efc/List>
#include <efc/Task>
#include <efc/Futex>


namespace EFC {

class PLATFORM_MAKE_PUBLIC TasksPool
{
public:
	TasksPool(int maxThreads);
	~TasksPool();

	bool haveFreeThreads() const;
	void handleImmediately(Task::Holder &task);
	void handle(Task::Holder &task);
    void cancel(const Task *task);
	void clear();

protected:
	friend class TaskThread;
	void nextTask(TaskThread *thread, Task *&task);

private:
	typedef List<Task *>                    Tasks;
	typedef List<TaskThread *>              Threads;
    typedef Map<const Task *, TaskThread *> BusyThreads;

private:
	mutable Futex m_futex;
	Tasks m_tasks;
	Threads m_threads;
	Threads m_freeThreads;
	BusyThreads m_busyThreads;
	const Threads::size_type m_maxThreads;
};

}

#endif /* EFC_TASKSPOOL_H_ */
