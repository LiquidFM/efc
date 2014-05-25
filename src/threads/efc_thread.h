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

#ifndef EFC_THREAD_H_
#define EFC_THREAD_H_

#include <efc/WaitCondition>


namespace EFC {

class PLATFORM_MAKE_PUBLIC Thread
{
    PLATFORM_MAKE_NONCOPYABLE(Thread)

public:
	enum Errors
	{
		NoError,
		PermissionDenied,
		InsufficientResources,
		InvalidSettings
	};

public:
    Thread();
    virtual ~Thread();

    thread_id_t threadId() const;
    static thread_id_t currentThreadId();

protected:
    class StartStopHandler
    {
    public:
    	StartStopHandler() :
    		m_run(false)
    	{}

    	volatile bool isRunning() const { return m_run; }

        const Mutex &mutex() const { return m_mutex; }
        Mutex &mutex() { return m_mutex; }

        inline void waitForStart()
        {
        	m_waitForStartStop.wait(m_mutex);
        }

        inline void waitForStop()
        {
        	m_waitForStartStop.wait(m_mutex);
        }

        inline void signalStarted()
        {
        	m_run = true;
            m_waitForStartStop.wakeAll();
        }

        inline void signalStopped()
        {
        	m_run = false;
            m_waitForStartStop.wakeAll();
        }

    private:
        bool m_run;
        Mutex m_mutex;
        WaitCondition m_waitForStartStop;
    };

protected:
    Errors start();
    void join();

    virtual void run() = 0;

private:
#if PLATFORM_OS(UNIX)
    static void *callback(void *argument);

private:
    thread_t m_thread;
#elif PLATFORM_OS(WINDOWS)
    static DWORD WINAPI callback(void *argument);

private:
    thread_t m_handle;
    thread_id_t m_thread;
#endif
};

}

#endif /* EFC_THREAD_H_ */
