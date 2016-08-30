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
#include <in6addr.h>
#include <mstcpip.h>
#include <ws2tcpip.h>


namespace EFC {

static int socket_family[Socket::Packet + 1] = { AF_INET, AF_INET6, AF_UNSPEC };
static int socket_type[Socket::Raw + 1] = { SOCK_STREAM, SOCK_DGRAM, SOCK_RAW };
static int socket_protocol[Socket::Packet + 1] = { 0, 0, 0 };


Socket::Socket(Family family, Type type, bool blockingMode) :
	LastError(),
	m_socket(InvalidDescriptor)
{
	WSADATA data;
	int res = ::WSAStartup(MAKEWORD(2, 2), &data);

	if (LIKELY(res == 0))
	{
		m_socket = ::socket(socket_family[family], socket_type[type], htons(socket_protocol[family]));

		if (UNLIKELY(m_socket == InvalidDescriptor))
		{
			setLastError(::WSAGetLastError());
			::WSACleanup();
		}
		else
		{
			char value = 1;
	        ::setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));

            if (!blockingMode)
            {
                u_long mode = 1;
                ::ioctlsocket(m_socket, FIONBIO, &mode);
            }
		}
	}
	else
		setLastError(res);
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

    char optval;
    socklen_t optlen;

    if (::getsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0)
    {
        setLastError(errno);
        return false;
    }

    enabled = optval;

    // see https://msdn.microsoft.com/en-us/library/windows/desktop/dd877220%28v=vs.85%29.aspx
    // see https://msdn.microsoft.com/en-us/library/windows/desktop/ms738544%28v=vs.85%29.aspx
    // time, interval is not reading values, probes always 10 (see http://stackoverflow.com/questions/18391341/reading-sio-keepalive-vals-fields-on-a-windows-socket-for-keepalive-idle-and-in)
    time = -1;
    interval = -1;
    probes = 10; // always 10

    return true;
}

bool Socket::setKeepalive(bool enabled, int time, int interval, int probes)
{
    ASSERT(isValid());

    tcp_keepalive keepAliveSettings;
    keepAliveSettings.onoff = (enabled ? 1 : 0);
    keepAliveSettings.keepalivetime = time;
    keepAliveSettings.keepaliveinterval = interval;

    // probes do not changed

    unsigned long bytesReturned = 0;
    if (WSAIoctl(m_socket, SIO_KEEPALIVE_VALS, &keepAliveSettings, sizeof(keepAliveSettings), NULL, 0, &bytesReturned, NULL, NULL) != 0)
    {
            setLastError(errno);
            return false;
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

    res = ::getaddrinfo(name, NULL, &hints, &result);

    if (UNLIKELY(res != 0))
    {
    	error = ::WSAGetLastError();
        return false;
    }

    if (UNLIKELY(result->ai_addr == NULL))
    {
    	error = ::WSAGetLastError();
        ::freeaddrinfo(result);

        return false;
    }

    if (UNLIKELY(result->ai_addrlen != sizeof(struct sockaddr_in)))
    {
    	error = ::WSAGetLastError();
        ::freeaddrinfo(result);

        return false;
    }

    in_addr = (struct sockaddr_in *)result->ai_addr;

    ::memcpy(reinterpret_cast<ULONG *>(&address), &in_addr->sin_addr.s_addr, sizeof(ULONG));
    ::freeaddrinfo(result);

    return true;
}

bool Socket::firstLocalAddress(String &address, error_code_t &error)
{
	EFC_CRITICAL("Not implemented!");
	return false;
}

int Socket::interfaceIndex(const String &interface, error_code_t &error)
{
	EFC_CRITICAL("Not implemented!");
	return InvalidInterfaceIndex;
}

String Socket::hostNameFromAddress(const address_t &address)
{
	return ::inet_ntoa(reinterpret_cast<const struct sockaddr_in &>(address).sin_addr);
}

Socket::in_port_t Socket::hostPortFromAddress(const address_t &address)
{
	return ntohs(reinterpret_cast<const struct sockaddr_in &>(address).sin_port);
}

void Socket::disconnect()
{
	::shutdown(m_socket, SD_BOTH);
}

void Socket::close()
{
	::closesocket(m_socket);
	m_socket = InvalidDescriptor;

	if (LIKELY(m_socket != InvalidDescriptor))
		::WSACleanup();
}

Socket::Socket(descriptor_t descriptor) :
	LastError(),
	m_socket(descriptor)
{}

}
