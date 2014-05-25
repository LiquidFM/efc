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

#include "../efc_thread.h"

#include <brolly/assert.h>
#include <signal.h>
#include <errno.h>


namespace EFC {

#if PLATFORM_OS(UNIX)
static const Thread::Errors errors[EINVAL + 1] =
{
	Thread::NoError, Thread::PermissionDenied,      Thread::NoError, Thread::NoError, Thread::NoError,
	Thread::NoError, Thread::NoError,               Thread::NoError, Thread::NoError, Thread::NoError,
	Thread::NoError, Thread::InsufficientResources, Thread::NoError, Thread::NoError, Thread::NoError,
	Thread::NoError, Thread::NoError,               Thread::NoError, Thread::NoError, Thread::NoError,
	Thread::NoError, Thread::NoError,               Thread::InvalidSettings
};
#else
#error This UNIX platform is not supported
#endif


static thread_t invalidThread()
{
	return 0;
}


Thread::Thread() :
	m_thread(invalidThread())
{
	ASSERT(errors[EAGAIN] == Thread::InsufficientResources);
	ASSERT(errors[EINVAL] == Thread::InvalidSettings);
	ASSERT(errors[EPERM] == Thread::PermissionDenied);
}

Thread::~Thread()
{
	join();
}

thread_id_t Thread::threadId() const
{
	return (thread_id_t)m_thread;
}

thread_id_t Thread::currentThreadId()
{
    return (thread_id_t)pthread_self();
}

Thread::Errors Thread::start()
{
	ASSERT(pthread_equal(m_thread, invalidThread()));
	int res = pthread_create(&m_thread, 0, callback, this);

	if (LIKELY(res == 0))
	{
		return NoError;
	}
	else
	{
		m_thread = invalidThread();
		return errors[res];
	}
}

void Thread::join()
{
	if (!pthread_equal(m_thread, invalidThread()))
	{
		void *ptr;
		pthread_join(m_thread, &ptr);
		m_thread = invalidThread();
	}
}

void *Thread::callback(void *argument)
{
    sigset_t sm;
    sigemptyset(&sm);
    pthread_sigmask(SIG_SETMASK, &sm, 0);

    static_cast<Thread *>(argument)->run();
    return NULL;
}

}
