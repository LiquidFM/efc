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

#ifndef EFC_TASKTHREAD_H_
#define EFC_TASKTHREAD_H_

#include <efc/Task>
#include <efc/Thread>


namespace EFC {
class TasksPool;


class PLATFORM_MAKE_PRIVATE TaskThread : public Thread
{
public:
    TaskThread(TasksPool *pool, Task *task, bool &vaild);
    virtual ~TaskThread();

    void handle(Task *task);
    void cancel(bool wait);

protected:
    virtual void run();

private:
    bool m_abort;
    bool m_cancel;
    Task *m_task;
    TasksPool *m_pool;
    WaitCondition m_condition;
    StartStopHandler m_handler;
};

}

#endif /* EFC_TASKTHREAD_H_ */
