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
#ifndef EFC_NETWORK_UDPSENDER_H_
#define EFC_NETWORK_UDPSENDER_H_

#include <efc/network/Transport>


namespace EFC {

class PLATFORM_MAKE_PUBLIC UdpSender : public Transport
{
public:
	UdpSender(Socket &socket);

	bool send(const char *address, in_port_t port, const void *buffer, size_t size);
};

}

#endif /* EFC_NETWORK_UDPSENDER_H_ */
