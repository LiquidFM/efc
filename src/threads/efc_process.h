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

#ifndef EFC_PROCESS_H_
#define EFC_PROCESS_H_

#include <platform/utils.h>
#include <efc/threads_common.h>


namespace EFC {

class PLATFORM_MAKE_PUBLIC Process
{
    PLATFORM_MAKE_NONCOPYABLE(Process)

public:
    Process();
#if PLATFORM_COMPILER_SUPPORTS(MOVE_SEMANTIC)
    Process(Process &&other);
#endif
    ~Process();

#if PLATFORM_COMPILER_SUPPORTS(MOVE_SEMANTIC)
    Process &operator=(Process &&other);
#endif

    static Process current();
    static Process create(const char *appName, char * const *cmdLine, const char *workingDir, bool useParentStdout);

public:
    bool isValid() const;

    process_id_t processId() const;
    static process_id_t currentProcessId();
    static process_id_t invalidProcessId();

public:
    void terminate();
    bool isTerminated();
    void waitTerminated();
    bool waitTerminated(unsigned int timeout);

public:
    bool suspend();
    bool resume();
    bool waitSuspended();

#if PLATFORM_OS(UNIX)
protected:
    Process(process_t pid);

private:
    process_t m_pid;
    bool m_terminated;
#elif PLATFORM_OS(WINDOWS)
protected:
    Process(process_t handle, process_id_t id);

private:
    process_t m_handle;
    process_id_t m_process;
#endif
};

}

#endif /* EFC_PROCESS_H_ */
