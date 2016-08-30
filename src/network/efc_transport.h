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
#ifndef EFC_NETWORK_TRANSPORT_H_
#define EFC_NETWORK_TRANSPORT_H_

#include <efc/network/Socket>


namespace EFC {

class PLATFORM_MAKE_PUBLIC Transport : public LastError
{
public:
	Transport(Socket &socket);

	bool isConnectionLost() const;
	bool isInterrupted() const;

	/**
	 * This is non-blocking function. So, you have to wait
	 * the end of read/write operations outside of this class.
	 */
	void interrupt();

protected:
	Socket::descriptor_t descriptor() const { return m_socket->descriptor(); }

	bool waitForRead();
	bool waitForWrite();
	bool waitForRead(int milliseconds, bool &timeout);

private:
	Socket *m_socket;
	bool m_interrupted;
};

}

#endif /* EFC_NETWORK_TRANSPORT_H_ */
