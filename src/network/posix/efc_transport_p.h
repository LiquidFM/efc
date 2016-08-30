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
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>


namespace EFC {

Transport::Transport(Socket &socket) :
	LastError(),
	m_socket(&socket),
	m_interrupted(false)
{}

bool Transport::isConnectionLost() const
{
	return lastError() == ECONNRESET ||
		   lastError() == ENOBUFS ||
		   lastError() == EPIPE;
}

bool Transport::isInterrupted() const
{
	return m_interrupted || lastError() == EINTR;
}

void Transport::interrupt()
{
	m_interrupted = true;
	m_socket->disconnect();
}

bool Transport::waitForRead()
{
    fd_set rfds;
    fd_set efds;
    int res;

    FD_ZERO(&rfds);
    FD_ZERO(&efds);

    FD_SET(descriptor(), &rfds);
    FD_SET(descriptor(), &efds);

    do
    	res = ::select(descriptor() + 1, &rfds, NULL, &efds, NULL);
    while (res < 0 && errno == EINTR);

    if (LIKELY(res > 0))
        if (UNLIKELY(FD_ISSET(descriptor(), &efds)))
        	setLastError(EIO);
        else
           	return true;
    else
    	setLastError(errno);

    return false;
}

bool Transport::waitForWrite()
{
    fd_set wfds;
    fd_set efds;
    int res;

    FD_ZERO(&wfds);
    FD_ZERO(&efds);

    FD_SET(descriptor(), &wfds);
    FD_SET(descriptor(), &efds);

    do
    	res = ::select(descriptor() + 1, NULL, &wfds, &efds, NULL);
    while (res < 0 && errno == EINTR);

    if (LIKELY(res > 0))
        if (UNLIKELY(FD_ISSET(descriptor(), &efds)))
        	setLastError(EIO);
        else
           	return true;
    else
    	setLastError(errno);

    return false;
}

bool Transport::waitForRead(int milliseconds, bool &timeout)
{
    fd_set rfds;
    fd_set efds;
    struct timeval tv;
    int res;

    FD_ZERO(&rfds);
    FD_ZERO(&efds);

    FD_SET(descriptor(), &rfds);
    FD_SET(descriptor(), &efds);

    tv.tv_sec = (long)(milliseconds / 1000);
    tv.tv_usec = (milliseconds % 1000) * 1000;

    do
    	res = ::select(descriptor() + 1, &rfds, NULL, &efds, &tv);
    while (res < 0 && errno == EINTR);

    if (LIKELY(res > 0))
        if (UNLIKELY(FD_ISSET(descriptor(), &efds)))
        	setLastError(EIO);
        else
           	return true;
    else
    	if (LIKELY(res == 0))
        	timeout = true;
    	else
    		setLastError(errno);

    return false;
}

}
