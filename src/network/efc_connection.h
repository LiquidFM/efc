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
#ifndef EFC_NETWORK_CONNECTION_H_
#define EFC_NETWORK_CONNECTION_H_

#include <efc/String>
#include <efc/network/Connection>
#include <efc/network/Receiver>
#include <efc/network/Sender>


namespace EFC {

class PLATFORM_MAKE_PUBLIC Connection : public Socket
{
	PLATFORM_MAKE_NONCOPYABLE(Connection)

public:
	Connection();
	Connection(RVALUE_REF(Connection) other);
	~Connection();

	Connection &operator=(RVALUE_REF(Connection) other);

	const Sender &sender() const { return m_sender; }
	Sender &sender() { return m_sender; }

	const Receiver &receiver() const { return m_receiver; }
	Receiver &receiver() { return m_receiver; }

	const String &remoteAddress() const { return m_remoteAddress; }
	in_port_t remotePort() const { return m_remotePort; }

	in_port_t localPort() const { return m_localPort; }

protected:
	void setRemotePeer(const char *address, in_port_t port) { m_remoteAddress = address; m_remotePort = port; }
	void setLocalPeer(in_port_t port) { m_localPort = port; }

protected:
	friend class ServerSocket;
	Connection(error_code_t error);
	Connection(descriptor_t descriptor, const address_t &remoteAddress, in_port_t localPort);

private:
	Sender m_sender;
	Receiver m_receiver;
	String m_remoteAddress;
	in_port_t m_remotePort;
	in_port_t m_localPort;
};

}

#endif /* EFC_NETWORK_CONNECTION_H_ */
