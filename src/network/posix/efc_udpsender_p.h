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

#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


namespace EFC {

UdpSender::UdpSender(Socket &socket) :
	Transport(socket)
{}

GCC_WARNING_OFF(strict-aliasing)

bool UdpSender::send(const char *address, in_port_t port, const void *buffer, size_t size)
{
	struct sockaddr_in saddr;
	ssize_t written;

	::memset(&saddr, 0, sizeof(struct sockaddr_in));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);

	if ((saddr.sin_addr.s_addr = ::inet_addr(address)) == (unsigned long)INADDR_NONE)
	    if (Socket::resolve(address, reinterpret_cast<Socket::in_address_t &>(saddr.sin_addr.s_addr), *this))
	    	written = ::sendto(descriptor(), buffer, size, MSG_NOSIGNAL, reinterpret_cast<struct sockaddr *>(&saddr), sizeof(struct sockaddr_in));
	    else
	    	return false;
	else
    	written = ::sendto(descriptor(), buffer, size, MSG_NOSIGNAL, reinterpret_cast<struct sockaddr *>(&saddr), sizeof(struct sockaddr_in));

    if (LIKELY(written > 0))
    {
		ssize_t res;

		while (static_cast<size_t>(written) < size)
		{
	    	res = ::sendto(descriptor(), static_cast<const char *>(buffer) + written, size - written, MSG_NOSIGNAL, reinterpret_cast<struct sockaddr *>(&saddr), sizeof(struct sockaddr_in));

		    if (LIKELY(res > 0))
				written += res;
		    else
		    {
		        if (LIKELY(res == 0))
		            setLastError(EINTR);
		        else
					setLastError(errno);

		        return false;
		    }
		}

		return true;
	}
    else
        if (LIKELY(written == 0))
            setLastError(EINTR);
        else
			setLastError(errno);

	return false;
}

GCC_WARNING_ON(strict-aliasing)

}
