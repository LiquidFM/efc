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

#ifndef EFC_ATOMICS_H_
#define EFC_ATOMICS_H_

#include <cstdint>


namespace EFC {

#define EFC_ATOMIC_INIT(i)	{ (i) }

struct atomic_t
{
	int counter;
};

void cpu_relax();
void barrier();

/**
 * An exchange-type operation, which takes a value and a pointer, and
 * returns the old value.
 */
uint8_t xchg(volatile uint8_t *ptr, uint8_t val);
int8_t xchg(volatile int8_t *ptr, int8_t val);
uint16_t xchg(volatile uint16_t *ptr, uint16_t val);
int16_t xchg(volatile int16_t *ptr, int16_t val);
uint32_t xchg(volatile uint32_t *ptr, uint32_t val);
int32_t xchg(volatile int32_t *ptr, int32_t val);
uint64_t xchg(volatile uint64_t *ptr, uint64_t val);
int64_t xchg(volatile int64_t *ptr, int64_t val);

/**
 * Atomic compare and exchange.  Compare OLD with MEM, if identical,
 * store NEW in MEM.  Return the initial value in MEM.  Success is
 * indicated by comparing RETURN with OLD.
 */
uint8_t cmpxchg(volatile uint8_t *ptr, uint8_t old_val, uint8_t new_val);
int8_t cmpxchg(volatile int8_t *ptr, int8_t old_val, int8_t new_val);
uint16_t cmpxchg(volatile uint16_t *ptr, uint16_t old_val, uint16_t new_val);
int16_t cmpxchg(volatile int16_t *ptr, int16_t old_val, int16_t new_val);
uint32_t cmpxchg(volatile uint32_t *ptr, uint32_t old_val, uint32_t new_val);
int32_t cmpxchg(volatile int32_t *ptr, int32_t old_val, int32_t new_val);
uint64_t cmpxchg(volatile uint64_t *ptr, uint64_t old_val, uint64_t new_val);
int64_t cmpxchg(volatile int64_t *ptr, int64_t old_val, int64_t new_val);
void *cmpxchg_ptr(volatile void **ptr, void *old_val, void *new_val);

/**
 * atomic_read - read atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically reads the value of @v.
 */
int atomic_read(const atomic_t *v);

/**
 * atomic_set - set atomic variable
 * @v: pointer of type atomic_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.
 */
void atomic_write(atomic_t *v, int i);

/**
 * atomic_inc - increment atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1.
 */
void atomic_inc(uint8_t *v);
void atomic_inc(int8_t *v);
void atomic_inc(uint16_t *v);
void atomic_inc(int16_t *v);
void atomic_inc(uint32_t *v);
void atomic_inc(int32_t *v);
void atomic_inc(uint64_t *v);
void atomic_inc(int64_t *v);
void atomic_inc(atomic_t *v);

/**
 * atomic_dec - decrement atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically decrements @v by 1.
 */
void atomic_dec(uint8_t *v);
void atomic_dec(int8_t *v);
void atomic_dec(uint16_t *v);
void atomic_dec(int16_t *v);
void atomic_dec(uint32_t *v);
void atomic_dec(int32_t *v);
void atomic_dec(uint64_t *v);
void atomic_dec(int64_t *v);
void atomic_dec(atomic_t *v);

/**
 * atomic_dec_and_test - decrement and test
 * @v: pointer of type atomic_t
 *
 * Atomically decrements @v by 1 and
 * returns true if the result is 0, or false for all other
 * cases.
 */
bool atomic_dec_and_test(atomic_t *v);

int atomic_xchg(atomic_t *v, int new_val);

int atomic_cmpxchg(atomic_t *v, int old_val, int new_val);

/**
 * atomic_add_unless - add unless the number is already a given value
 * @v: pointer of type atomic_t
 * @a: the amount to add to v...
 * @u: ...unless v is equal to u.
 *
 * Atomically adds @a to @v, so long as @v was not already @u.
 * Returns the old value of @v.
 */
int atomic_add_unless(atomic_t *v, int a, int u);

}

#endif /* EFC_ATOMICS_H_ */
