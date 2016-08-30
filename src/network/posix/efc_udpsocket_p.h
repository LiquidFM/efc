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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>


namespace EFC {

UdpSocket::UdpSocket(in_port_t port) :
	Socket(IPv4, Datagram),
	m_sender(*this),
	m_receiver(*this),
	m_localPort(port)
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
	    	setLastError(errno);
	    	close();
	    }
	}
}

UdpSocket::UdpSocket(const String &address, in_port_t port) :
	Socket(IPv4, Datagram),
	m_sender(*this),
	m_receiver(*this),
	m_localAddress(address),
	m_localPort(port)
{
	ASSERT(!m_localAddress.empty());

	if (LIKELY(isValid() == true))
	{
		struct sockaddr_in saddr;
		int res;

		::memset(&saddr, 0, sizeof(struct sockaddr_in));
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(m_localPort);

		res = inet_aton(m_localAddress.c_str(), &saddr.sin_addr);

		if (UNLIKELY(res == 0))
		{
	    	setLastError(errno);
	    	close();
		}
		else
		{
			res = ::bind(descriptor(), reinterpret_cast<struct sockaddr *>(&saddr), sizeof(struct sockaddr_in));

			if (UNLIKELY(res < 0))
			{
				setLastError(errno);
				close();
			}
		}
	}
}

}
