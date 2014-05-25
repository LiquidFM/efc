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

#include <brolly/assert.h>


namespace EFC {

TlsValue::TlsValue()
{
	if (UNLIKELY(::pthread_key_create(&m_key, NULL) != 0))
		ASSERT(!"Can't create TLS key!");
}

TlsValue::~TlsValue()
{
	if (UNLIKELY(::pthread_key_delete(m_key) != 0))
		ASSERT(!"Can't destroy TLS key!");
}

void TlsValue::set(void *value)
{
	if (UNLIKELY(::pthread_setspecific(m_key, value) != 0))
		ASSERT(!"Can't set TLS value!");
}

void *TlsValue::get() const
{
	return ::pthread_getspecific(m_key);
}

}
