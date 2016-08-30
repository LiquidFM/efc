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
#ifndef EFC_NETWORK_TCPSERVER_H_
#define EFC_NETWORK_TCPSERVER_H_

#include <efc/network/Socket>
#include <efc/network/Connection>


namespace EFC {

class PLATFORM_MAKE_PUBLIC ServerSocket : public Socket
{
	PLATFORM_MAKE_NONCOPYABLE(ServerSocket)

public:
	ServerSocket(in_port_t port, int backlog, Family family = IPv4, Type type = Stream, bool blockingMode = true);
	ServerSocket(const String &address, in_port_t port, int backlog, Family family = IPv4, Type type = Stream, bool blockingMode = true);
	ServerSocket(RVALUE_REF(ServerSocket) other);
	~ServerSocket();

	ServerSocket &operator=(RVALUE_REF(ServerSocket) other);

	const String &localAddress() const { return m_localAddress; }
	in_port_t localPort() const { return m_localPort; }

	bool isTerminated() const { return m_terminated; }
	Connection accept(bool blockingMode = true);
	void abort();

private:
	String m_localAddress;
	in_port_t m_localPort;
	bool m_terminating;
	bool m_terminated;
};

}

#endif /* EFC_NETWORK_TCPSERVER_H_ */
