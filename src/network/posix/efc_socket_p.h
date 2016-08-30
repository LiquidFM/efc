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
#include <netinet/tcp.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <net/ethernet.h>
#include <ifaddrs.h>


namespace EFC {

static int socket_family[Socket::Packet + 1] = { AF_INET, AF_INET6, AF_PACKET };
static int socket_type[Socket::Raw + 1] = { SOCK_STREAM, SOCK_DGRAM, SOCK_RAW };
static int socket_protocol[Socket::Packet + 1] = { 0, 0, ETH_P_ALL };
static int socket_mode[2] = { SOCK_NONBLOCK, 0 };


Socket::Socket(Family family, Type type, bool blockingMode) :
	LastError(),
	m_socket(::socket(socket_family[family], socket_type[type] | socket_mode[blockingMode], htons(socket_protocol[family])))
{
    if (UNLIKELY(m_socket == InvalidDescriptor))
    	setLastError(errno);
    else
    {
    	int value = 1;
        ::setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));
    }
}

Socket::Socket(RVALUE_REF(Socket) other) :
	LastError(RVALUE_VAL(other)),
	m_socket(RVALUE_VAL(other).m_socket)
{
    RVALUE_VAL(other).m_socket = InvalidDescriptor;
}

Socket::~Socket()
{
	close();
}

Socket &Socket::operator=(RVALUE_REF(Socket) other)
{
	LastError::operator=(RVALUE_VAL(other));

	close();
	m_socket = RVALUE_VAL(other).m_socket;
	RVALUE_VAL(other).m_socket = InvalidDescriptor;

	return *this;
}

bool Socket::keepalive(bool &enabled, int &time, int &interval, int &probes) const
{
    ASSERT(isValid());

    int optval;
    socklen_t optlen;

    if (::getsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0)
    {
        setLastError(errno);
        return false;
    }

    enabled = optval;

    if (::getsockopt(m_socket, SOL_TCP, TCP_KEEPCNT, &optval, &optlen) < 0)
    {
        setLastError(errno);
        return false;
    }

    probes = optval;

    if (::getsockopt(m_socket, SOL_TCP, TCP_KEEPIDLE, &optval, &optlen) < 0)
    {
        setLastError(errno);
        return false;
    }

    time = optval;

    if (::getsockopt(m_socket, SOL_TCP, TCP_KEEPINTVL, &optval, &optlen) < 0)
    {
        setLastError(errno);
        return false;
    }

    interval = optval;

    return true;
}

bool Socket::setKeepalive(bool enabled, int time, int interval, int probes)
{
    ASSERT(isValid());

    int optval = enabled;
    if (::setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) < 0)
    {
        setLastError(errno);
        return false;
    }

    if (enabled)
    {
        if (::setsockopt(m_socket, SOL_TCP, TCP_KEEPCNT, &probes, sizeof(probes)) < 0)
        {
            setLastError(errno);
            return false;
        }

        if (::setsockopt(m_socket, SOL_TCP, TCP_KEEPIDLE, &time, sizeof(time)) < 0)
        {
            setLastError(errno);
            return false;
        }

        if (::setsockopt(m_socket, SOL_TCP, TCP_KEEPINTVL, &interval, sizeof(interval)) < 0)
        {
            setLastError(errno);
            return false;
        }
    }

    return true;
}

bool Socket::resolve(const char *name, in_address_t &address, error_code_t &error)
{
    struct addrinfo hints;
    struct addrinfo *result = NULL;
    struct sockaddr_in* in_addr;
    int res;

    ::memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       /* Allow IPv4  */
    hints.ai_socktype = SOCK_STREAM; /* Stream socket */

    res = getaddrinfo(name, NULL, &hints, &result);

    if (UNLIKELY(res != 0))
    {
    	error = errno;
        return false;
    }

    if (UNLIKELY(result->ai_addr == NULL))
    {
    	error = errno;
        ::freeaddrinfo(result);

        return false;
    }

    if (UNLIKELY(result->ai_addrlen != sizeof(struct sockaddr_in)))
    {
    	error = errno;
        ::freeaddrinfo(result);

        return false;
    }

    in_addr = (struct sockaddr_in *)result->ai_addr;

    ::memcpy(reinterpret_cast<in_addr_t *>(&address), &in_addr->sin_addr.s_addr, sizeof(in_addr_t));
    ::freeaddrinfo(result);

    return true;
}

bool Socket::firstLocalAddress(String &address, error_code_t &error)
{
    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];
    int res;

    res = ::getifaddrs(&ifaddr);

    if (UNLIKELY(res == -1))
    {
    	error = errno;
    	return false;
    }

    /* Walk through linked list, maintaining head pointer so we
       can free list later */

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
    	if (ifa->ifa_addr == NULL)
    		continue;

        /* For an AF_INET* interface address, save the address */

        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            res = ::getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

            if (UNLIKELY(res != 0))
            {
            	error = errno;
                ::freeifaddrs(ifaddr);

            	return false;
            }

            address = host;
            ::freeifaddrs(ifaddr);

            return true;
        }
    }

    ::freeifaddrs(ifaddr);
	return false;
}

int Socket::interfaceIndex(const String &interface, error_code_t &error)
{
    int res = ::if_nametoindex(interface.c_str());

    if (LIKELY(res > 0))
    	return res;
    else
    	error = errno;

    return InvalidInterfaceIndex;
}

String Socket::hostNameFromAddress(const address_t &address)
{
	return ::inet_ntoa(reinterpret_cast<const struct sockaddr_in &>(address).sin_addr);
}

in_port_t Socket::hostPortFromAddress(const address_t &address)
{
	return ntohs(reinterpret_cast<const struct sockaddr_in &>(address).sin_port);
}

void Socket::disconnect()
{
	::shutdown(m_socket, SHUT_RDWR);
}

void Socket::close()
{
	::close(m_socket);
	m_socket = InvalidDescriptor;
}

Socket::Socket(descriptor_t descriptor) :
	LastError(),
	m_socket(descriptor)
{}

}
