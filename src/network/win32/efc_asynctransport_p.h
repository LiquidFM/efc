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
#include <brolly/assert.h>
#include <ws2tcpip.h>
#include <errno.h>


namespace EFC {

AsyncTransport::AsyncTransport() :
	LastError()
{}

bool AsyncTransport::handle(Socket *socket)
{
	ASSERT(socket != NULL);
	m_container[socket->descriptor()] = socket;
    
    return true;
}

void AsyncTransport::remove(Socket *socket)
{
	ASSERT(socket != NULL);
	m_container.erase(socket->descriptor());
}

Socket *AsyncTransport::waitForRead()
{
	Socket::descriptor_t descriptor = Socket::InvalidDescriptor;
    fd_set rfds;
    fd_set efds;
    int res;

    FD_ZERO(&rfds);
    FD_ZERO(&efds);

	setLastError(NoError);

    for (Container::const_iterator i = m_container.begin(), end = m_container.end(); i != end; ++i)
    {
    	if ((*i).second->descriptor() > descriptor)
    		descriptor = (*i).second->descriptor();

    	FD_SET((*i).second->descriptor(), &rfds);
    	FD_SET((*i).second->descriptor(), &efds);
    }

    do
    	res = ::select(descriptor + 1, &rfds, NULL, &efds, NULL);
    while (res < 0 && errno == EINTR);

    if (LIKELY(res > 0))
        for (Container::const_iterator i = m_container.begin(), end = m_container.end(); i != end; ++i)
        {
            if (FD_ISSET((*i).second->descriptor(), &rfds))
				return (*i).second;
            else
				if (UNLIKELY(FD_ISSET((*i).second->descriptor(), &efds)))
				{
					setLastError(EIO);
					return (*i).second;
				}
        }
    else
    	setLastError(errno);

    return NULL;
}

Socket *AsyncTransport::waitForWrite()
{
	Socket::descriptor_t descriptor = Socket::InvalidDescriptor;
    fd_set wfds;
    fd_set efds;
    int res;

    FD_ZERO(&wfds);
    FD_ZERO(&efds);

	setLastError(NoError);

    for (Container::const_iterator i = m_container.begin(), end = m_container.end(); i != end; ++i)
    {
    	if ((*i).second->descriptor() > descriptor)
    		descriptor = (*i).second->descriptor();

    	FD_SET((*i).second->descriptor(), &wfds);
    	FD_SET((*i).second->descriptor(), &efds);
    }

    do
    	res = ::select(descriptor + 1, NULL, &wfds, &efds, NULL);
    while (res < 0 && errno == EINTR);

    if (LIKELY(res > 0))
        for (Container::const_iterator i = m_container.begin(), end = m_container.end(); i != end; ++i)
        {
            if (FD_ISSET((*i).second->descriptor(), &wfds))
				return (*i).second;
            else
				if (UNLIKELY(FD_ISSET((*i).second->descriptor(), &efds)))
				{
					setLastError(EIO);
					return (*i).second;
				}
        }
    else
    	setLastError(errno);

    return NULL;
}

Socket *AsyncTransport::waitForRead(int milliseconds, bool &timeout)
{
	Socket::descriptor_t descriptor = Socket::InvalidDescriptor;
    fd_set rfds;
    fd_set efds;
    struct timeval tv;
    int res;

    FD_ZERO(&rfds);
    FD_ZERO(&efds);

    tv.tv_sec = (long)(milliseconds / 1000);
    tv.tv_usec = (milliseconds % 1000) * 1000;

    timeout = false;
    setLastError(NoError);

    for (Container::const_iterator i = m_container.begin(), end = m_container.end(); i != end; ++i)
    {
    	if ((*i).second->descriptor() > descriptor)
    		descriptor = (*i).second->descriptor();

    	FD_SET((*i).second->descriptor(), &rfds);
    	FD_SET((*i).second->descriptor(), &efds);
    }

    do
    	res = ::select(descriptor + 1, &rfds, NULL, &efds, &tv);
    while (res < 0 && errno == EINTR);

    if (LIKELY(res > 0))
        for (Container::const_iterator i = m_container.begin(), end = m_container.end(); i != end; ++i)
        {
            if (FD_ISSET((*i).second->descriptor(), &rfds))
				return (*i).second;
            else
				if (UNLIKELY(FD_ISSET((*i).second->descriptor(), &efds)))
				{
					setLastError(EIO);
					return (*i).second;
				}
        }
    else
    	if (LIKELY(res == 0))
        	timeout = true;
    	else
    		setLastError(errno);

    return NULL;
}

}
