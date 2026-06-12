#ifndef CLIENTSOCK_H
#define CLIENTSOCK_H

#include <string>
#include <string.h>
#include "cleansocket.h"

namespace Celeris
{
	class ClientSocket
	{
		public:
			ClientSocket();
			~ClientSocket() noexcept;
			ClientSocket(const ClientSocket&) = delete;
			ClientSocket& operator = (const ClientSocket&) = delete;

			ClientSocket(ClientSocket&&) noexcept = default;
			ClientSocket& operator = (ClientSocket&&) noexcept = default;


			std::string sendGetRequestWithSocket(CleanSocket* tcpSocketIPV4, char origin[]);

		private:
	};
}

#endif // !CLIENTSOCK_H
