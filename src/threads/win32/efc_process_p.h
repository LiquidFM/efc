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

#include <cstdlib>
#include <windows.h>


namespace EFC {

enum
{
	InvalidProcessId = -1
};


Process::Process() :
	m_handle(NULL),
	m_process(InvalidProcessId)
{}

#if PLATFORM_COMPILER_SUPPORTS(MOVE_SEMANTIC)
Process::Process(Process &&other) :
	m_handle(other.m_handle),
	m_process(other.m_process)
{
	other.m_handle = NULL;
	other.m_process = InvalidProcessId;
}
#endif

Process::~Process()
{}

#if PLATFORM_COMPILER_SUPPORTS(MOVE_SEMANTIC)
Process &Process::operator=(Process &&other)
{
	m_handle = other.m_handle;
	other.m_handle = NULL;
	m_process = other.m_process;
	other.m_process = InvalidProcessId;

	return *this;
}
#endif

Process Process::current()
{
	return Process(NULL, InvalidProcessId);
}

Process Process::create(const char *appName, char * const *cmdLine, const char *workingDir, bool useParentStdIO)
{
	ASSERT(!"Not implemented yet!");

	return Process(NULL, InvalidProcessId);
}

bool Process::isValid() const
{
	return (m_handle != NULL);
}

process_id_t Process::processId() const
{
	return m_process;
}

process_id_t Process::currentProcessId()
{
	return ::GetCurrentProcessId();
}

process_id_t Process::invalidProcessId()
{
	return InvalidProcessId;
}

void Process::terminate()
{
	ASSERT(m_handle != NULL);

	ASSERT(!"Not implemented yet!");
}

bool Process::isTerminated()
{
	ASSERT(m_handle != NULL);

	ASSERT(!"Not implemented yet!");

	return true;
}

void Process::waitTerminated()
{
	ASSERT(m_handle != NULL);

	ASSERT(!"Not implemented yet!");
}

bool Process::waitTerminated(unsigned int timeout)
{
    ASSERT(m_handle != NULL);

    ASSERT(!"Not implemented yet!");

    return true;
}

bool Process::suspend()
{
	ASSERT(m_handle != NULL);

	ASSERT(!"Not implemented yet!");

	return false;
}

bool Process::resume()
{
	ASSERT(m_handle != NULL);

	ASSERT(!"Not implemented yet!");

	return false;
}

bool Process::waitSuspended()
{
	ASSERT(m_handle != NULL);

	ASSERT(!"Not implemented yet!");

	return false;
}

Process::Process(process_t handle, process_id_t id) :
	m_handle(handle),
	m_process(id)
{}

}
