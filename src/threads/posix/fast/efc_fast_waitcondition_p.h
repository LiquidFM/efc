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

#ifndef EFC_FAST_WAITCONDITION_P_H_
#define EFC_FAST_WAITCONDITION_P_H_

#include "efc_linux_futex_p.h"


namespace EFC {

WaitCondition::WaitCondition() :
	m_condition()
{}

WaitCondition::~WaitCondition()
{}

void WaitCondition::wakeOne()
{
	/* We are waking someone up */
	atomic_inc(&m_condition.seq);

	/* Wake up a thread */
	sys_futex(&m_condition.seq, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, 0);
}

void WaitCondition::wakeAll()
{
	mutex_t *m = m_condition.m;

	/* No mutex means that there are no waiters */
	if (!m)
		return;

	/* We are waking everyone up */
	atomic_inc(&m_condition.seq);

	/* Wake one thread, and requeue the rest on the mutex */
	sys_futex(&m_condition.seq, FUTEX_REQUEUE_PRIVATE, 1, (timespec *)INT_MAX, m, 0);
}

void WaitCondition::wait(Mutex &mutex)
{
	int seq = m_condition.seq;

	if (m_condition.m != &mutex.m_mutex)
	{
		if (m_condition.m)
			return;

		/* Atomically set mutex */
		cmpxchg_ptr((volatile void **)&m_condition.m, NULL, &mutex.m_mutex);

		if (m_condition.m != &mutex.m_mutex)
			return;
	}

	mutex.unlock();

	sys_futex(&m_condition.seq, FUTEX_WAIT_PRIVATE, seq, NULL, NULL, 0);

	while (xchg(&mutex.m_mutex, 2))
		sys_futex(&mutex.m_mutex, FUTEX_WAIT_PRIVATE, 2, NULL, NULL, 0);
}

bool WaitCondition::wait(Mutex &mutex, unsigned msec)
{
	int seq = m_condition.seq;

	if (m_condition.m != &mutex.m_mutex)
	{
		if (m_condition.m)
			return false;

		/* Atomically set mutex */
		cmpxchg_ptr((volatile void **)&m_condition.m, NULL, &mutex.m_mutex);

		if (m_condition.m != &mutex.m_mutex)
			return false;
	}

	mutex.unlock();

	unsigned sec = msec / 1000;
	struct timespec ts = { static_cast<time_t>(sec),  static_cast<time_t>(((msec - sec * 1000) * 1000) * 1000) };
	int res = sys_futex(&m_condition.seq, FUTEX_WAIT_PRIVATE, seq, &ts, NULL, 0);
	int error = errno;

	while (xchg(&mutex.m_mutex, 2))
		sys_futex(&mutex.m_mutex, FUTEX_WAIT_PRIVATE, 2, NULL, NULL, 0);

	if (res == 0)
		return true;
	else
		if (LIKELY(error == ETIMEDOUT))
			return false;
		else if (LIKELY(error == EINVAL))
			return false;
		else
			ASSERT(!"Can't wait on condition variable!");

	return false;
}

}

#endif /* EFC_FAST_WAITCONDITION_P_H_ */
