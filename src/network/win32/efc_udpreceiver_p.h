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
#include <ws2tcpip.h>
#include <errno.h>


namespace EFC {

UdpReceiver::UdpReceiver(Socket &socket) :
	Transport(socket)
{}

bool UdpReceiver::receive(void *buffer, size_t size, size_t &received)
{
    if (LIKELY(waitForRead() == true))
	{
    	int res = ::recvfrom(descriptor(), static_cast<char *>(buffer), size, 0, NULL, NULL);

        if (LIKELY(res > 0))
        {
    		received = res;
    		return true;
        }
        else
            if (LIKELY(res == 0))
                setLastError(EINTR);
            else
    			setLastError(::WSAGetLastError());
	}

    received = 0;
    return false;
}

bool UdpReceiver::receive(void *buffer, size_t size, size_t &received, String &peerName, Socket::in_port_t &peerPort)
{
    if (LIKELY(waitForRead() == true))
	{
    	socklen_t addrlen = sizeof(struct sockaddr_in);
    	struct sockaddr_in addr;

    	int res = ::recvfrom(descriptor(), static_cast<char *>(buffer), size, 0, reinterpret_cast<struct sockaddr *>(&addr), &addrlen);

        if (LIKELY(res > 0))
        {
			if (LIKELY(addrlen > 0))
			{
				peerName = Socket::hostNameFromAddress(reinterpret_cast<Socket::address_t &>(addr));
				peerPort = Socket::hostPortFromAddress(reinterpret_cast<Socket::address_t &>(addr));
			}
			else
			{
				peerName.clear();
				peerPort = 0;
			}

			received = res;
    		return true;
        }
        else
            if (LIKELY(res == 0))
                setLastError(EINTR);
            else
    			setLastError(::WSAGetLastError());
	}

    received = 0;
    return false;
}

}
