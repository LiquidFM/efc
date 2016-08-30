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
#ifndef EFC_NETWORK_LASTERROR_H_
#define EFC_NETWORK_LASTERROR_H_

#include <efc/move>
#include <platform/utils.h>


namespace EFC {

class PLATFORM_MAKE_PUBLIC LastError
{
public:
	typedef int error_code_t;
	enum { NoError = 0 };

public:
	LastError(error_code_t error = NoError) :
		m_lastError(error)
	{}

	error_code_t lastError() const { return m_lastError; }
	static const char *errorDescription(error_code_t error);

protected:
	operator const error_code_t &() const { return m_lastError; }
	operator error_code_t &() { return m_lastError; }

	void setLastError(error_code_t error) const { m_lastError = error; }

private:
	mutable error_code_t m_lastError;
};

}

#endif /* EFC_NETWORK_LASTERROR_H_ */
