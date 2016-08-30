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
#ifndef EFC_NETWORK_SOCKET_H_
#define EFC_NETWORK_SOCKET_H_

#include <platform/utils.h>

#if PLATFORM_OS(WINDOWS)

#if PLATFORM_COMPILER(MINGW)
#include <winsock2.h>

#ifdef interface
#undef interface
#endif

#else
#include <WinSock2.h>
#endif
#else
#include <netinet/in.h>
#endif

#include <efc/String>
#include <efc/network/LastError>


namespace EFC {

class PLATFORM_MAKE_PUBLIC Socket : public LastError
{
	PLATFORM_MAKE_NONCOPYABLE(Socket)

public:
#if PLATFORM_OS(UNIX)
	typedef int            descriptor_t;
	enum { InvalidDescriptor = -1 };
#elif PLATFORM_OS(WINDOWS)
	typedef ::SOCKET       descriptor_t;
	enum { InvalidDescriptor = INVALID_SOCKET };
#endif

	typedef struct {}      in_address_t;
	typedef struct {}      address_t;
    typedef unsigned char  byte_t;
	enum { InvalidInterfaceIndex = -1 };

	enum Family
	{
		IPv4,
		IPv6,
		Packet
	};

	enum Type
	{
		Stream,
		Datagram,
		Raw
	};

public:
	Socket(Family family = IPv4, Type type = Stream, bool blockingMode = true);
	Socket(RVALUE_REF(Socket) other);
	~Socket();

	Socket &operator=(RVALUE_REF(Socket) other);

	bool isValid() const { return m_socket != Socket::InvalidDescriptor; }

	/**
	 * @param enabled enable/disable keepalive packets
	 * @param time the interval between the last data packet sent (simple ACKs are not considered data) and the first keepalive probe
	 * @param interval the interval between subsequential keepalive probes, regardless of what the connection has exchanged in the meantime
	 * @param probes the number of unacknowledged probes to send before considering the connection dead and notifying the application layer
	 */
	bool keepalive(bool &enabled, int &time, int &interval, int &probes) const;

    /**
     * @param enabled enable/disable keepalive packets
     * @param time the interval between the last data packet sent (simple ACKs are not considered data) and the first keepalive probe
     * @param interval the interval between subsequential keepalive probes, regardless of what the connection has exchanged in the meantime
     * @param probes the number of unacknowledged probes to send before considering the connection dead and notifying the application layer
     */
	bool setKeepalive(bool enabled, int time, int interval, int probes);

	static bool resolve(const char *name, in_address_t &address, error_code_t &error);
	static bool firstLocalAddress(String &address, error_code_t &error);
	static int interfaceIndex(const String &interface, error_code_t &error);
	static String hostNameFromAddress(const address_t &address);
	static in_port_t hostPortFromAddress(const address_t &address);

protected:
	friend class Transport;
	friend class AsyncTransport;
	descriptor_t descriptor() const { return m_socket; }
	void disconnect();
	void close();

protected:
	Socket(descriptor_t descriptor);

private:
	descriptor_t m_socket;
};

}

#endif /* EFC_NETWORK_SOCKET_H_ */
