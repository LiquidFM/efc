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

#ifndef EFC_TLSVALUE_H_
#define EFC_TLSVALUE_H_

#include <platform/utils.h>
#include <efc/threads_common.h>


namespace EFC {

class PLATFORM_MAKE_PUBLIC TlsValue
{
	PLATFORM_MAKE_NONCOPYABLE(TlsValue)
	PLATFORM_MAKE_NONMOVEABLE(TlsValue)

public:
    TlsValue();
    ~TlsValue();

    void set(void *value);
    void *get() const;

    template <typename R> inline
    R *as() { return static_cast<R *>(get()); }

    template <typename R> inline
    const R *as() const { return static_cast<const R *>(get()); }

    template <typename R> inline
    const R *as_const() const { return static_cast<const R *>(get()); }

private:
    tls_key_t m_key;
};

}

#endif /* EFC_TLSVALUE_H_ */
