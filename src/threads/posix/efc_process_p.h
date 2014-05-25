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
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>


namespace EFC {

enum
{
	InvalidProcessId = -1
};


Process::Process() :
	m_pid(InvalidProcessId)
{}

#if PLATFORM_COMPILER_SUPPORTS(MOVE_SEMANTIC)
Process::Process(Process &&other) :
	m_pid(other.m_pid)
{
	other.m_pid = InvalidProcessId;
}
#endif

Process::~Process()
{}

#if PLATFORM_COMPILER_SUPPORTS(MOVE_SEMANTIC)
Process &Process::operator=(Process &&other)
{
	m_pid = other.m_pid;
	other.m_pid = InvalidProcessId;

	return *this;
}
#endif

Process Process::current()
{
	return Process(::getpid());
}

Process Process::create(const char *appName, char * const *cmdLine, const char *workingDir, bool useParentStdIO)
{
	pid_t pid = fork();

	if (pid == 0)
	{
		//obtain a new process group
		if (::setsid() < 0)
		{
			::_exit(EXIT_FAILURE);
		}

		//change current directory
		if (::chdir(workingDir) < 0)
		{
			::_exit(EXIT_FAILURE);
		}

		if (useParentStdIO)
		{
			//close all inherited descriptors (except stdio)
			for (int fd = ::getdtablesize(); fd >= 0; --fd)
			{
				if (fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
				{
					::close(fd);
				}
			}
		}
		else
		{
			//close all inherited descriptors
			for (int fd = ::getdtablesize(); fd >= 0; --fd)
			{
				::close(fd);
			}

			//for safety, standart I/O should be opened and connected to a harmless I/O device
			int fd = ::open("/dev/null", O_RDWR);

			if (UNLIKELY(fd < 0))
			{
				::_exit(EXIT_FAILURE);
			}

			if (::dup2(fd, STDOUT_FILENO) < 0)
			{
				::_exit(EXIT_FAILURE);
			}

			if (::dup2(fd, STDERR_FILENO) < 0)
			{
				::_exit(EXIT_FAILURE);
			}
		}

		//for safety, prevent unsecure file priviliges that may occur on file creation
		::umask(027);

		::execv(appName, cmdLine);

		::_exit(EXIT_FAILURE);
	}
	else if (LIKELY(pid > 0))
	{
		return Process(pid);
	}

	return Process(InvalidProcessId);
}

bool Process::isValid() const
{
	return (m_pid != InvalidProcessId);
}

process_id_t Process::processId() const
{
	return m_pid;
}

process_id_t Process::currentProcessId()
{
	return ::getpid();
}

process_id_t Process::invalidProcessId()
{
	return InvalidProcessId;
}

void Process::terminate()
{
	ASSERT(m_pid != InvalidProcessId);

	if (::kill(m_pid, SIGKILL) < 0)
	{
		if (errno == ESRCH)
		{
			return; //terminated yet
		}
		else
		{
			ASSERT(!"kill return error!");	// crash
		}
	}
}

bool Process::isTerminated()
{
	ASSERT(m_pid != InvalidProcessId);

	int wstatus = 0;
	pid_t wpid = ::waitpid(m_pid, &wstatus, WNOHANG);

	bool terminated = false;
	if (wpid < 0)
	{
		if (errno == ECHILD)
		{
			m_pid = InvalidProcessId; //invalidate process id
			terminated = true;
		}
		else
		{
			ASSERT(!"waitpid return error!");	// crash
		}
	}
	else if (wpid == 0)
	{
		terminated = false;
	}
	else if (wpid != m_pid)
	{
		ASSERT(!"waitpid return another process id!");	// crash
	}
	else
	{
		m_pid = InvalidProcessId; //invalidate process id
		terminated = true;
	}

	return terminated;
}

void Process::waitTerminated()
{
	ASSERT(m_pid != InvalidProcessId);

	int wstatus = 0;
	pid_t wpid = ::waitpid(m_pid, &wstatus, 0);

	if (wpid < 0)
	{
		if (errno == ECHILD)
		{
			m_pid = InvalidProcessId; //invalidate process id
		}
		else
		{
			ASSERT(!"waitpid return error!");	// crash
		}
	}
	else if (wpid == 0)
	{
		ASSERT(!"waitpid return zero process id!");	// crash
	}
	else if (wpid != m_pid)
	{
		ASSERT(!"waitpid return another process id!");	// crash
	}
	else
	{
		m_pid = InvalidProcessId; //invalidate process id
	}
}

bool Process::waitTerminated(unsigned int timeout)
{
	bool terminated = false;

	unsigned int terminateTimeout = timeout;		// milliseconds
	unsigned int sleepTimeout = 10;					// milliseconds
	unsigned int efforts = terminateTimeout / sleepTimeout;

	for (unsigned int i = 0; i <= efforts && !terminated; ++i)
	{
		::EFC::ThisThread::sleep(sleepTimeout);
		terminated = isTerminated();
	}

	return terminated;
}

bool Process::suspend()
{
	ASSERT(m_pid != InvalidProcessId);

	if (::kill(m_pid, SIGSTOP) < 0)
	{
		if (errno == ESRCH || errno == EPERM)
		{
			return false;
		}
		else
		{
			ASSERT(!"kill return error!");	// crash
		}
	}

	return true;
}

bool Process::resume()
{
	ASSERT(m_pid != InvalidProcessId);

	if (::kill(m_pid, SIGCONT) < 0)
	{
		if (errno == ESRCH || errno == EPERM)
		{
			return false;
		}
		else
		{
			ASSERT(!"kill return error!");	// crash
		}
	}

	return true;
}

bool Process::waitSuspended()
{
	ASSERT(m_pid != InvalidProcessId);

	int wstatus = 0;
	pid_t wpid = ::waitpid(m_pid, &wstatus, WUNTRACED);

	bool suspended = false;
	if (wpid < 0)
	{
		if (errno == ECHILD)
		{
			//terminated, not suspended
			m_pid = InvalidProcessId; //invalidate process id
			suspended = false;
		}
		else
		{
			ASSERT(!"waitpid return error!");	// crash
		}
	}
	else if (wpid == 0)
	{
		ASSERT(!"waitpid return zero process id!");	// crash
	}
	else if (wpid != m_pid)
	{
		ASSERT(!"waitpid return another process id!");	// crash
	}
	else
	{
		if (WIFSTOPPED(wstatus))
		{
			ASSERT(WSTOPSIG(wstatus) == SIGSTOP);
			suspended = true;
		}
		else
		{
			//terminated, not suspended
			m_pid = InvalidProcessId; //invalidate process id
			suspended = false;
		}
	}

	return suspended;
}

Process::Process(process_t pid) :
	m_pid(pid)
{}

}
