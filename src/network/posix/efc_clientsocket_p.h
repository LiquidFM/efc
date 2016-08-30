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

#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>


namespace EFC {

ClientSocket::ClientSocket() :
	Connection()
{}

ClientSocket::ClientSocket(RVALUE_REF(ClientSocket) other) :
	Connection(std::move(other))
{}

ClientSocket::~ClientSocket()
{}

ClientSocket &ClientSocket::operator=(RVALUE_REF(ClientSocket) other)
{
	Connection::operator=(std::move(other));
	return *this;
}

GCC_WARNING_OFF(strict-aliasing)

bool ClientSocket::connect(in_addr_t remoteAddress, in_port_t remotePort)
{
    if (remoteAddress <= 0 || remotePort <= 0)
    {
        setLastError(EINVAL);
        return false;
    }

    struct sockaddr_in saddr;

    ::memset(&saddr, 0, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    saddr.sin_port = remotePort;
    saddr.sin_addr.s_addr = remoteAddress;

    if (::connect(descriptor(), reinterpret_cast<struct sockaddr *>(&saddr), sizeof(struct sockaddr_in)) < 0)
        setLastError(errno);
    else
        return true;

    return false;
}

bool ClientSocket::connect(const char *remoteAddress, in_port_t remotePort)
{
    if (remoteAddress == NULL)
    {
        setLastError(EINVAL);
        return false;
    }

    in_addr_t addr;

	if ((addr = ::inet_addr(remoteAddress)) == (unsigned long)INADDR_NONE)
		if (!resolve(remoteAddress, reinterpret_cast<in_address_t &>(addr), *this))
			return false;

	if (connect(addr, htons(remotePort)))
	{
		setRemotePeer(remoteAddress, remotePort);
		return true;
	}

	return false;
}

bool ClientSocket::connect(const char *remoteAddress, in_port_t remotePort, in_port_t localPort)
{
    if (localPort <= 0)
    {
        setLastError(EINVAL);
        return false;
    }

    struct sockaddr_in saddr;

	::memset(&saddr, 0, sizeof(struct sockaddr_in));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(localPort);

	if (::bind(descriptor(), reinterpret_cast<struct sockaddr *>(&saddr), sizeof(struct sockaddr_in)) < 0)
		setLastError(errno);
	else
	{
		setLocalPeer(localPort);
		return connect(remoteAddress, remotePort);
	}

	return false;
}

GCC_WARNING_ON(strict-aliasing)

}
