#include "net/clientsock.h"

Celeris::ClientSocket::~ClientSocket() noexcept
{
}
Celeris::ClientSocket::ClientSocket()
{
}

std::string Celeris::ClientSocket::sendGetRequestWithSocket(CleanSocket* tcpSocketIPV4, char origin[]) {
	addrinfo hints{};

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	addrinfo* results{};

	const char* port = "80";

	if (getaddrinfo(origin, "80", &hints, &results) != 0) {
		//TODO: handle error
		//return 1;
	}

	sockaddr_in Address{};
	memcpy_s(&Address, sizeof(Address), results->ai_addr, results->ai_addrlen);


	freeaddrinfo(results);

	connect(tcpSocketIPV4->Get(), reinterpret_cast<const sockaddr*>(&Address), sizeof(Address));

	char request[1024] = "GET / HTTP/1.1\r\n";
	strcat_s(request, origin);
	strcat_s(request, "Connection: close\r\nUser-Agent:TestAgent\r\n\r\n");

	send(tcpSocketIPV4->Get(), request, sizeof(request), 0);

	char buffer[1024]{};
	recv(tcpSocketIPV4->Get(), buffer, sizeof(buffer), 0);

	return buffer;
}