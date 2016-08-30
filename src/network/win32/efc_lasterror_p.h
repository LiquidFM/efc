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
#if PLATFORM_COMPILER(MINGW)
#include <winsock2.h>
#endif
#include <windows.h>
#include <string.h>


namespace EFC {

const char *LastError::errorDescription(error_code_t error)
{
	enum { BufferSize = 2048 };
	static char buffer[BufferSize];
	static const char default_error[] = "Failed to get error description";
	LPSTR errorText;

	DWORD res = ::FormatMessageA(
			// Use system message tables to retrieve error text
			FORMAT_MESSAGE_FROM_SYSTEM |
			// The function allocates a buffer large enough to hold the formatted message
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			// Insert sequences in the message definition are to be ignored and passed through to the output buffer unchanged
			FORMAT_MESSAGE_IGNORE_INSERTS,
			// Unused with FORMAT_MESSAGE_FROM_SYSTEM
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
			// Output
			(LPSTR)&errorText,
			0,
			NULL);

	if (res && res < BufferSize)
	{
		::memcpy(buffer, errorText, res);
		buffer[res] = 0;
		::LocalFree(errorText);
		return buffer;
	}

	return default_error;
}

}
