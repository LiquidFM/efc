/**
 * This file is part of the EFC library.
 *
 * Copyright (C) 2013 Dmitriy Vilkov, <dav.daemon@gmail.com>
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

#ifndef EFC_ATOMICS_P_H_
#define EFC_ATOMICS_P_H_

#define CONFIG_SMP

#ifdef CONFIG_SMP
#	define LOCK_PREFIX "lock; "
#else
#	define LOCK_PREFIX ""
#endif

#include <brolly/assert.h>


namespace EFC {

void cpu_relax()
{
	__asm__ __volatile__ ("pause\n": : :"memory");
}

void barrier()
{
//	__asm__ __volatile__("lock; addl $0,0(%esp)");
	volatile unsigned i;
	xchg(&i, 100);
}

/**
 * Note: no "lock" prefix even on SMP: xchg always implies lock anyway.
 * Since this is generally used to protect other memory information, we
 * use "__asm__ __volatile__" and "memory" clobbers to prevent gcc from moving
 * information around.
 */
uint8_t xchg(volatile uint8_t *ptr, uint8_t val)
{
	uint8_t res = val;

	__asm__ __volatile__ ("xchgb %b0, %1\n"
		      : "+q" (res), "+m" (*ptr)
		      : : "memory", "cc");

	return res;
}

int8_t xchg(volatile int8_t *ptr, int8_t val)
{
	int8_t res = val;

	__asm__ __volatile__ ("xchgb %b0, %1\n"
		      : "+q" (res), "+m" (*ptr)
		      : : "memory", "cc");

	return res;
}

uint16_t xchg(volatile uint16_t *ptr, uint16_t val)
{
	uint16_t res = val;

	__asm__ __volatile__ ("xchgw %w0, %1\n"
		      : "+r" (res), "+m" (*ptr)
		      : : "memory", "cc");

	return res;
}

int16_t xchg(volatile int16_t *ptr, int16_t val)
{
	int16_t res = val;

	__asm__ __volatile__ ("xchgw %w0, %1\n"
		      : "+r" (res), "+m" (*ptr)
		      : : "memory", "cc");

	return res;
}

uint32_t xchg(volatile uint32_t *ptr, uint32_t val)
{
	uint32_t res = val;

	__asm__ __volatile__ ("xchgl %0, %1\n"
		      : "+r" (res), "+m" (*ptr)
		      : : "memory", "cc");

	return res;
}

int32_t xchg(volatile int32_t *ptr, int32_t val)
{
	int32_t res = val;

	__asm__ __volatile__ ("xchgl %0, %1\n"
		      : "+r" (res), "+m" (*ptr)
		      : : "memory", "cc");

	return res;
}

#if PLATFORM_CPU(X86_64)
uint64_t xchg(volatile uint64_t *ptr, uint64_t val)
{
	uint64_t res = val;

	__asm__ __volatile__ ("xchgq %q0, %1\n"
		      : "+r" (res), "+m" (*ptr)
		      : : "memory", "cc");

	return res;
}

int64_t xchg(volatile int64_t *ptr, int64_t val)
{
	int64_t res = val;

	__asm__ __volatile__ ("xchgq %q0, %1\n"
		      : "+r" (res), "+m" (*ptr)
		      : : "memory", "cc");

	return res;
}
#else
uint64_t xchg(volatile uint64_t *ptr, uint64_t val)
{
	ASSERT(!"Not implemented!");
	return val;
}

int64_t xchg(volatile int64_t *ptr, int64_t val)
{
	ASSERT(!"Not implemented!");
	return val;
}
#endif

uint8_t cmpxchg(volatile uint8_t *ptr, uint8_t old_val, uint8_t new_val)
{
	uint8_t res;

	__asm__ __volatile__ (LOCK_PREFIX "cmpxchgb %2,%1"
			 : "=a" (res), "+m" (*ptr)
			 : "q" (new_val), "0" (old_val)
			 : "memory");

	return res;
}

int8_t cmpxchg(volatile int8_t *ptr, int8_t old_val, int8_t new_val)
{
	int8_t res;

	__asm__ __volatile__ (LOCK_PREFIX "cmpxchgb %2,%1"
			 : "=a" (res), "+m" (*ptr)
			 : "q" (new_val), "0" (old_val)
			 : "memory");

	return res;
}

uint16_t cmpxchg(volatile uint16_t *ptr, uint16_t old_val, uint16_t new_val)
{
	uint16_t res;

	__asm__ __volatile__ (LOCK_PREFIX "cmpxchgw %2,%1"
		     : "=a" (res), "+m" (*ptr)
		     : "r" (new_val), "0" (old_val)
		     : "memory");

	return res;
}

int16_t cmpxchg(volatile int16_t *ptr, int16_t old_val, int16_t new_val)
{
	int16_t res;

	__asm__ __volatile__ (LOCK_PREFIX "cmpxchgw %2,%1"
		     : "=a" (res), "+m" (*ptr)
		     : "r" (new_val), "0" (old_val)
		     : "memory");

	return res;
}

uint32_t cmpxchg(volatile uint32_t *ptr, uint32_t old_val, uint32_t new_val)
{
	uint32_t res;

	__asm__ __volatile__ (LOCK_PREFIX "cmpxchgl %2,%1"
		     : "=a" (res), "+m" (*ptr)
		     : "r" (new_val), "0" (old_val)
		     : "memory");

	return res;
}

int32_t cmpxchg(volatile int32_t *ptr, int32_t old_val, int32_t new_val)
{
	int32_t res;

	__asm__ __volatile__ (LOCK_PREFIX "cmpxchgl %2,%1"
		     : "=a" (res), "+m" (*ptr)
		     : "r" (new_val), "0" (old_val)
		     : "memory");

	return res;
}

#if PLATFORM_CPU(X86_64)
uint64_t cmpxchg(volatile uint64_t *ptr, uint64_t old_val, uint64_t new_val)
{
	uint64_t res;

	__asm__ __volatile__ (LOCK_PREFIX "cmpxchgq %2,%1"
		     : "=a" (res), "+m" (*ptr)
		     : "r" (new_val), "0" (old_val)
		     : "memory");

	return res;
}

int64_t cmpxchg(volatile int64_t *ptr, int64_t old_val, int64_t new_val)
{
	int64_t res;

	__asm__ __volatile__ (LOCK_PREFIX "cmpxchgq %2,%1"
		     : "=a" (res), "+m" (*ptr)
		     : "r" (new_val), "0" (old_val)
		     : "memory");

	return res;
}
#else
uint64_t cmpxchg(volatile uint64_t *ptr, uint64_t old_val, uint64_t new_val)
{
	uint64_t res;

	__asm__ __volatile__ (LOCK_PREFIX "cmpxchg8b %1"
		     : "=A" (res),
		       "+m" (*ptr)
		     : "b" ((uint32_t)new_val),
		       "c" ((uint32_t)(new_val >> 32)),
		       "0" (old_val)
		     : "memory");

	return res;
}

int64_t cmpxchg(volatile int64_t *ptr, int64_t old_val, int64_t new_val)
{
	int64_t res;

	__asm__ __volatile__ (LOCK_PREFIX "cmpxchg8b %1"
		     : "=A" (res),
		       "+m" (*ptr)
		     : "b" ((uint32_t)new_val),
		       "c" ((uint32_t)(new_val >> 32)),
		       "0" (old_val)
		     : "memory");

	return res;
}
#endif

void *cmpxchg_ptr(volatile void **ptr, void *old_val, void *new_val)
{
#if PLATFORM_CPU(X86_64)
	return (void *)cmpxchg((uint64_t *)ptr, (uint64_t)old_val, (uint64_t)new_val);
#else
	return (void *)cmpxchg((uint32_t *)ptr, (uint32_t)old_val, (uint32_t)new_val);
#endif
}

int atomic_read(const atomic_t *v)
{
	return (*(const __volatile__ int *)&(v)->counter);
}

void atomic_write(atomic_t *v, int i)
{
	v->counter = i;
}

void atomic_inc(uint8_t *v)
{
	__asm__ __volatile__ (LOCK_PREFIX "incb %0" : "+m" (*v));
}

void atomic_inc(int8_t *v)
{
	__asm__ __volatile__ (LOCK_PREFIX "incb %0" : "+m" (*v));
}

void atomic_inc(uint16_t *v)
{
	__asm__ __volatile__ (LOCK_PREFIX "incw %0" : "+m" (*v));
}

void atomic_inc(int16_t *v)
{
	__asm__ __volatile__ (LOCK_PREFIX "incw %0" : "+m" (*v));
}

void atomic_inc(uint32_t *v)
{
	__asm__ __volatile__ (LOCK_PREFIX "incl %0" : "+m" (*v));
}

void atomic_inc(int32_t *v)
{
	__asm__ __volatile__ (LOCK_PREFIX "incl %0" : "+m" (*v));
}

void atomic_inc(uint64_t *v)
{
#if PLATFORM_CPU(X86_64)
	__asm__ __volatile__ (LOCK_PREFIX "incq %0" : "+m" (*v));
#else
	ASSERT(!"Not implemented!");
#endif
}

void atomic_inc(int64_t *v)
{
#if PLATFORM_CPU(X86_64)
	__asm__ __volatile__ (LOCK_PREFIX "incq %0" : "+m" (*v));
#else
	ASSERT(!"Not implemented!");
#endif
}

void atomic_inc(atomic_t *v)
{
	atomic_inc(&v->counter);
}

void atomic_dec(uint8_t *v)
{
	__asm__ __volatile__ (LOCK_PREFIX "decb %0" : "+m" (*v));
}

void atomic_dec(int8_t *v)
{
	__asm__ __volatile__ (LOCK_PREFIX "decb %0" : "+m" (*v));
}

void atomic_dec(uint16_t *v)
{
	__asm__ __volatile__ (LOCK_PREFIX "decw %0" : "+m" (*v));
}

void atomic_dec(int16_t *v)
{
	__asm__ __volatile__ (LOCK_PREFIX "decw %0" : "+m" (*v));
}

void atomic_dec(uint32_t *v)
{
	__asm__ __volatile__ (LOCK_PREFIX "decl %0" : "+m" (*v));
}

void atomic_dec(int32_t *v)
{
	__asm__ __volatile__ (LOCK_PREFIX "decl %0" : "+m" (*v));
}

void atomic_dec(uint64_t *v)
{
#if PLATFORM_CPU(X86_64)
	__asm__ __volatile__ (LOCK_PREFIX "decq %0" : "+m" (*v));
#else
	ASSERT(!"Not implemented!");
#endif
}

void atomic_dec(int64_t *v)
{
#if PLATFORM_CPU(X86_64)
	__asm__ __volatile__ (LOCK_PREFIX "decq %0" : "+m" (*v));
#else
	ASSERT(!"Not implemented!");
#endif
}

void atomic_dec(atomic_t *v)
{
	atomic_dec(&v->counter);
}

bool atomic_dec_and_test(atomic_t *v)
{
	unsigned char c;

	__asm__ __volatile__ (LOCK_PREFIX "decl %0; sete %1"
		     : "+m" (v->counter), "=qm" (c)
		     : : "memory");

	return c != 0;
}

int atomic_xchg(atomic_t *v, int new_val)
{
	return xchg(&v->counter, new_val);
}

int atomic_cmpxchg(atomic_t *v, int old_val, int new_val)
{
	return cmpxchg(&v->counter, old_val, new_val);
}

int atomic_add_unless(atomic_t *v, int a, int u)
{
	int c, old;
	c = atomic_read(v);

	for (;;)
	{
		if (UNLIKELY(c == (u)))
			break;

		old = atomic_cmpxchg((v), c, c + (a));

		if (LIKELY(old == c))
			break;

		c = old;
	}

	return c;
}

}

#endif /* EFC_ATOMICS_P_H_ */
