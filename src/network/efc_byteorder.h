/**
 * This file is part of the EFC library.
 *
 * Copyright (C) 2011-2016 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * The EFC library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The EFC library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the EFC library. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef EFC_BYTEORDER_H_
#define EFC_BYTEORDER_H_

#include <platform/utils.h>
#include <platform/stdint.h>


#if PLATFORM_OS(UNIX)
#include <arpa/inet.h>
#elif PLATFORM_OS(WINDOWS)
#include <winsock.h>
#endif


inline uint64_t htonll(uint64_t value)
{
    return PLATFORM_MAKE_QWORD(htonl(PLATFORM_HI_DWORD(value)), htonl(PLATFORM_LO_DWORD(value)));
}

inline uint64_t ntohll(uint64_t value)
{
    return PLATFORM_MAKE_QWORD(ntohl(PLATFORM_HI_DWORD(value)), ntohl(PLATFORM_LO_DWORD(value)));
}

#if PLATFORM_COMPILER(GCC)
    GCC_WARNING_OFF(strict-aliasing)
#endif

inline uint32_t htonf(float value)
{
    uint32_t r = *reinterpret_cast<uint32_t *>(&value);
    return htonl(r);
}

inline float ntohf(uint32_t value)
{
    uint32_t r = ntohl(value);
    return *reinterpret_cast<float *>(&r);
}

inline uint64_t htond(double value)
{
    uint64_t r = *reinterpret_cast<uint64_t *>(&value);
    return htonll(r);
}

inline double ntohd(uint64_t value)
{
    uint64_t r = ntohll(value);
    return *reinterpret_cast<double *>(&r);
}

#if PLATFORM_COMPILER(GCC)
    GCC_WARNING_ON(strict-aliasing)
#endif

#endif /* EFC_BYTEORDER_H_ */
