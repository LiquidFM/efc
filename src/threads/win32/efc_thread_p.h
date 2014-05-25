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

Thread::Thread() :
	m_handle(NULL),
	m_thread(0)
{}

Thread::~Thread()
{
	join();
}

thread_id_t Thread::threadId() const
{
	return m_thread;
}

thread_id_t Thread::currentThreadId()
{
    return ::GetCurrentThreadId();
}

Thread::Errors Thread::start()
{
	ASSERT(m_handle == NULL);
	m_handle = ::CreateThread(NULL, 0, callback, this, 0, &m_thread);

	if (LIKELY(m_handle != NULL))
		return NoError;
	else
	{
		m_handle = NULL;
		return InsufficientResources;
	}
}

void Thread::join()
{
	if (m_handle)
	{
		if (UNLIKELY(::WaitForSingleObject(m_handle, INFINITE) != WAIT_OBJECT_0))
			ASSERT(!"Can't wait termination of Thread!");

		::CloseHandle(m_handle);
		m_handle = NULL;
	}
}

DWORD WINAPI Thread::callback(void *argument)
{
    ThreadData::Holder holder;
    ThreadData data(&holder);

    static_cast<Thread *>(argument)->run();

    return 0;
}

}
