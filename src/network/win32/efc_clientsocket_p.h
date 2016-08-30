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
#include <ws2tcpip.h>


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

bool ClientSocket::connect(const char *remoteAddress, in_port_t remotePort)
{
	struct sockaddr_in saddr;
	int res;

	::memset(&saddr, 0, sizeof(struct sockaddr_in));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(remotePort);

	if ((saddr.sin_addr.s_addr = ::inet_addr(remoteAddress)) == (unsigned long)INADDR_NONE)
		if (resolve(remoteAddress, reinterpret_cast<in_address_t &>(saddr.sin_addr.s_addr), *this))
			res = ::connect(descriptor(), reinterpret_cast<struct sockaddr *>(&saddr), sizeof(struct sockaddr_in));
		else
			return false;
	else
		res = ::connect(descriptor(), reinterpret_cast<struct sockaddr *>(&saddr), sizeof(struct sockaddr_in));

	if (UNLIKELY(res < 0))
		setLastError(::WSAGetLastError());
	else
	{
		setRemotePeer(remoteAddress, remotePort);
		return true;
	}

	return false;
}

bool ClientSocket::connect(const char *remoteAddress, in_port_t remotePort, in_port_t localPort)
{
	struct sockaddr_in saddr;
	int res;

	::memset(&saddr, 0, sizeof(struct sockaddr_in));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(localPort);

	res = ::bind(descriptor(), reinterpret_cast<struct sockaddr *>(&saddr), sizeof(struct sockaddr_in));

	if (UNLIKELY(res < 0))
		setLastError(::WSAGetLastError());
	else
	{
		setLocalPeer(localPort);
		return connect(remoteAddress, remotePort);
	}

	return false;
}

}
