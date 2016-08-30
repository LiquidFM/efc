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

#include <ws2tcpip.h>
#include <errno.h>


namespace EFC {

UdpSender::UdpSender(Socket &socket) :
	Transport(socket)
{}

bool UdpSender::send(const char *address, Socket::in_port_t port, const void *buffer, size_t size)
{
	struct sockaddr_in saddr;
	int written;

	::memset(&saddr, 0, sizeof(struct sockaddr_in));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);

	if ((saddr.sin_addr.s_addr = ::inet_addr(address)) == (unsigned long)INADDR_NONE)
	    if (Socket::resolve(address, reinterpret_cast<Socket::in_address_t &>(saddr.sin_addr.s_addr), *this))
	    	written = ::sendto(descriptor(), static_cast<const char *>(buffer), size, 0, reinterpret_cast<struct sockaddr *>(&saddr), sizeof(struct sockaddr_in));
	    else
	    	return false;
	else
    	written = ::sendto(descriptor(), static_cast<const char *>(buffer), size, 0, reinterpret_cast<struct sockaddr *>(&saddr), sizeof(struct sockaddr_in));

    if (LIKELY(written > 0))
    {
		int res;

		while (static_cast<size_t>(written) < size)
		{
	    	res = ::sendto(descriptor(), static_cast<const char *>(buffer) + written, size - written, 0, reinterpret_cast<struct sockaddr *>(&saddr), sizeof(struct sockaddr_in));

		    if (LIKELY(res > 0))
				written += res;
		    else
		    {
		        if (LIKELY(res == 0))
		            setLastError(EINTR);
		        else
					setLastError(::WSAGetLastError());

		        return false;
		    }
		}

		return true;
	}
    else
        if (LIKELY(written == 0))
            setLastError(EINTR);
        else
			setLastError(::WSAGetLastError());

	return false;
}

}
