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
namespace EFC {

Connection::Connection() :
	Socket(),
	m_sender(*this),
	m_receiver(*this),
	m_remotePort(0),
	m_localPort(0)
{}

Connection::Connection(RVALUE_REF(Connection) other) :
	Socket(std::move(other)),
	m_sender(*this),
	m_receiver(*this),
	m_remoteAddress(std::move(RVALUE_VAL(other).m_remoteAddress)),
	m_remotePort(RVALUE_VAL(other).m_remotePort),
	m_localPort(RVALUE_VAL(other).m_localPort)
{}

Connection::~Connection()
{}

Connection &Connection::operator=(RVALUE_REF(Connection) other)
{
	Socket::operator=(std::move(other));

	m_remoteAddress = std::move(RVALUE_VAL(other).m_remoteAddress);
	m_remotePort = RVALUE_VAL(other).m_remotePort;
	m_localPort = RVALUE_VAL(other).m_localPort;

	return *this;
}

Connection::Connection(error_code_t error) :
	Socket(InvalidDescriptor),
	m_sender(*this),
	m_receiver(*this),
	m_remotePort(0),
	m_localPort(0)
{
	setLastError(error);
}

Connection::Connection(descriptor_t descriptor, const address_t &remoteAddress, in_port_t localPort) :
	Socket(descriptor),
	m_sender(*this),
	m_receiver(*this),
	m_remoteAddress(hostNameFromAddress(remoteAddress)),
	m_remotePort(hostPortFromAddress(remoteAddress)),
	m_localPort(localPort)
{}

}
