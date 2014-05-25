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

#ifndef EFC_THREADS_COMMON_H_
#define EFC_THREADS_COMMON_H_

#include <platform/utils.h>


/* Preconditions */
#if PLATFORM_USE(FAST_MUTEXES)

#if !PLATFORM_OS(LINUX)
#error Fast mutexes is supported only on Linux platform
#endif

#if PLATFORM_ENABLE(DEADLOCKS_DETECTION)
#error Fast mutexes does not support deadlocks detection
#endif

#endif


/* Platform dependent declarations */
#if PLATFORM_OS(UNIX)
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <sched.h>


namespace EFC {

#if PLATFORM_USE(FAST_MUTEXES)
	typedef uint32_t mutex_t;
	struct condition_t
	{
		mutex_t *m;
		unsigned seq;
		unsigned pad;
	};
#else
	typedef pthread_mutex_t mutex_t;
	typedef pthread_cond_t  condition_t;
#endif

	typedef pthread_t     thread_t;
	typedef pthread_t     thread_id_t;
	typedef pid_t     	  process_t;
	typedef pid_t         process_id_t;
	typedef pthread_key_t tls_key_t;
	typedef uint32_t      duration_t;


namespace ThisThread
{
    inline void yield() 
    {
        ::sched_yield();
    }
    
    inline void sleep(duration_t msec)
    {
        useconds_t t = msec * 1000;
        ::usleep(t);
    }
}

}

#elif PLATFORM_OS(WINDOWS)
#include <windows.h>
#include <stdint.h>


namespace EFC {

typedef HANDLE mutex_t;
struct PLATFORM_MAKE_PUBLIC condition_t
{
	/**
	* Number of waiting threads.
	*/
	int waiters_count;

	/**
	* Serialize access to waiters_count_.
	*/
	CRITICAL_SECTION waiters_count_lock;

	/**
	* Semaphore used to queue up threads waiting for the condition to
	* become signaled.
	*/
	HANDLE semaphore;

	/**
	* An auto-reset event used by the broadcast/signal thread to wait
	* for all the waiting thread(s) to wake up and be released from the
	* semaphore.
	*/
	HANDLE waiters_done;

	/* Keeps track of whether we were broadcasting or signaling.
	* This
	* allows us to optimize the code if we're just signaling.
	*/
	size_t was_broadcast;
};

typedef HANDLE   thread_t;
typedef DWORD    thread_id_t;
typedef HANDLE   process_t;
typedef DWORD    process_id_t;
typedef DWORD    tls_key_t;
typedef uint32_t duration_t;


namespace ThisThread
{
#if defined(Yield)
#undef Yield
#endif

    inline void yield() 
    {
#if defined(_WIN32_WINNT) && _WIN32_WINNT >= 0x0400
        ::SwitchToThread();
#else
        ::Sleep(0);
#endif
    }
    
    inline void sleep(duration_t msec)
    {
        ::Sleep(msec);
    }
}

}

#endif

#endif /* EFC_THREADS_COMMON_H_ */
