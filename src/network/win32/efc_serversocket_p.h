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
#include "../efc_byteorder.h"

#include <brolly/assert.h>
#include <string.h>

#include <ws2tcpip.h>
#include <errno.h>


namespace EFC {

ServerSocket::ServerSocket(in_port_t port, int backlog, Family family, Type type, bool blockingMode) :
	Socket(family, type, blockingMode),
	m_localPort(port),
	m_terminating(false),
	m_terminated(false)
{
	if (LIKELY(isValid() == true))
	{
		struct sockaddr_in saddr;
		int res;

		::memset(&saddr, 0, sizeof(struct sockaddr_in));
		saddr.sin_family = AF_INET;
		saddr.sin_addr.s_addr = htonl(INADDR_ANY);
		saddr.sin_port = htons(m_localPort);

		res = ::bind(descriptor(), reinterpret_cast<struct sockaddr *>(&saddr), sizeof(struct sockaddr_in));

	    if (UNLIKELY(res < 0))
	    {
	    	setLastError(::WSAGetLastError());
	    	close();
	    }
	    else
	    {
	    	res = ::listen(descriptor(), backlog);

			if (UNLIKELY(res < 0))
			{
				setLastError(::WSAGetLastError());
		    	close();
			}
	    }
	}
}

ServerSocket::ServerSocket(const String &address, in_port_t port, int backlog, Family family, Type type, bool blockingMode) :
	Socket(family, type, blockingMode),
	m_localAddress(address),
	m_localPort(port),
	m_terminating(false),
	m_terminated(false)
{
	ASSERT(!m_localAddress.empty());

	if (LIKELY(isValid() == true))
	{
		struct sockaddr_in saddr;
		int res;

		::memset(&saddr, 0, sizeof(struct sockaddr_in));
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(m_localPort);
		saddr.sin_addr.s_addr = ::inet_addr(m_localAddress.c_str());

		if (UNLIKELY(saddr.sin_addr.s_addr == INADDR_NONE))
		{
	    	setLastError(EINVAL);
	    	close();
		}
		else
		{
			res = ::bind(descriptor(), reinterpret_cast<struct sockaddr *>(&saddr), sizeof(struct sockaddr_in));

			if (UNLIKELY(res < 0))
			{
				setLastError(::WSAGetLastError());
				close();
			}
			else
			{
				res = ::listen(descriptor(), backlog);

				if (UNLIKELY(res < 0))
				{
					setLastError(::WSAGetLastError());
					close();
				}
			}
		}
	}
}

ServerSocket::ServerSocket(RVALUE_REF(ServerSocket) other) :
	Socket(std::move(other)),
    m_localAddress(std::move(RVALUE_VAL(other).m_localAddress)),
	m_localPort(RVALUE_VAL(other).m_localPort),
	m_terminating(RVALUE_VAL(other).m_terminating),
	m_terminated(RVALUE_VAL(other).m_terminated)
{}

ServerSocket::~ServerSocket()
{}

ServerSocket &ServerSocket::operator=(RVALUE_REF(ServerSocket) other)
{
	Socket::operator=(std::move(other));

	m_localAddress = std::move(RVALUE_VAL(other).m_localAddress);
	m_localPort = RVALUE_VAL(other).m_localPort;
	m_terminating = RVALUE_VAL(other).m_terminating;
	m_terminated = RVALUE_VAL(other).m_terminated;

	return *this;
}

Connection ServerSocket::accept(bool blockingMode)
{
	ASSERT(isValid());
	descriptor_t desc;
    socklen_t peerlen = sizeof(struct sockaddr_in);
    struct sockaddr_in peeraddr;

    desc = ::accept(descriptor(), reinterpret_cast<struct sockaddr *>(&peeraddr), &peerlen);

    if (!blockingMode)
    {
        u_long iMode = 1;
        ::ioctlsocket(descriptor(), FIONBIO, &iMode);
    }
        
    if (UNLIKELY(desc == INVALID_SOCKET))
    	if (LIKELY(m_terminating == true))
    	{
    		m_terminated = true;
    		return Connection(EINTR);
    	}
    	else
    		return Connection(::WSAGetLastError());

    return Connection(desc, reinterpret_cast<const address_t &>(peeraddr), localPort());
}

void ServerSocket::abort()
{
	m_terminating = true;
	close();
}

}
