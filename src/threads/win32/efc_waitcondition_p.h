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

WaitCondition::WaitCondition() :
	m_condition()
{
	m_condition.waiters_count = 0;
	m_condition.was_broadcast = 0;
	m_condition.semaphore = ::CreateSemaphore(NULL, 0, 0x7fffffff, NULL);

	::InitializeCriticalSection(&m_condition.waiters_count_lock);
	m_condition.waiters_done = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

WaitCondition::~WaitCondition()
{
	::CloseHandle(m_condition.semaphore);
	::CloseHandle(m_condition.waiters_done);
	::DeleteCriticalSection(&m_condition.waiters_count_lock);
}

void WaitCondition::wakeOne()
{
	::EnterCriticalSection(&m_condition.waiters_count_lock);
	int have_waiters = m_condition.waiters_count > 0;
	::LeaveCriticalSection(&m_condition.waiters_count_lock);

	// If there aren't any waiters, then this is a no-op.
	if (have_waiters)
		::ReleaseSemaphore(m_condition.semaphore, 1, 0);
}

void WaitCondition::wakeAll()
{
	// This is needed to ensure that waiters_count and was_broadcast are
	// consistent relative to each other.
	::EnterCriticalSection(&m_condition.waiters_count_lock);
	int have_waiters = 0;

	if (m_condition.waiters_count > 0)
	{
		// We are broadcasting, even if there is just one waiter...
		// Record that we are broadcasting, which helps optimize
		// wait() for the non-broadcast case.
		m_condition.was_broadcast = 1;
		have_waiters = 1;
	}

	if (have_waiters)
	{
		// Wake up all the waiters atomically.
		::ReleaseSemaphore(m_condition.semaphore, m_condition.waiters_count, 0);

		::LeaveCriticalSection(&m_condition.waiters_count_lock);

		// Wait for all the awakened threads to acquire the counting
		// semaphore.
		::WaitForSingleObject(m_condition.waiters_done, INFINITE);
		// This assignment is okay, even without the waiters_count_lock held
		// because no other waiter threads can wake up to access it.
		m_condition.was_broadcast = 0;
	}
	else
		::LeaveCriticalSection(&m_condition.waiters_count_lock);
}

void WaitCondition::wait(Mutex &mutex)
{
	// Avoid race conditions.
	::EnterCriticalSection(&m_condition.waiters_count_lock);
	++m_condition.waiters_count;
	::LeaveCriticalSection(&m_condition.waiters_count_lock);

	// This call atomically releases the mutex and waits on the
	// semaphore until wakeOne() or wakeAll() are called by another thread.
	::SignalObjectAndWait(mutex.m_mutex, m_condition.semaphore, INFINITE, FALSE);

	// Reacquire lock to avoid race conditions.
	::EnterCriticalSection(&m_condition.waiters_count_lock);

	// We're no longer waiting...
	--m_condition.waiters_count;

	// Check to see if we're the last waiter after wakeAll().
	int last_waiter = m_condition.was_broadcast && m_condition.waiters_count == 0;

	::LeaveCriticalSection(&m_condition.waiters_count_lock);

	// If we're the last waiter thread during this particular broadcast
	// then let all the other threads proceed.
	if (last_waiter)
		// This call atomically signals the waiters_done event and waits until
		// it can acquire the mutex. This is required to ensure fairness.
		::SignalObjectAndWait(m_condition.waiters_done, mutex.m_mutex, INFINITE, FALSE);
	else
		// Always regain the external mutex since that's the guarantee we
		// give to our callers.
		::WaitForSingleObject(mutex.m_mutex, INFINITE);
}

bool WaitCondition::wait(Mutex &mutex, unsigned msec)
{
	ASSERT(!"Not implemented!");
	return false;
}

}
