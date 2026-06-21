#include "net/clientsock.h"
#include <cstring>

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
	auto addr_size = sizeof(Address);

	//test comparison of sizes (will possibly move to a better system for memory size checks )

	if (addr_size >= static_cast<size_t>(results->ai_addrlen))
	{
		std::memcpy(&Address,results->ai_addr,results->ai_addrlen);
	}
	//memcpy_s(&Address, sizeof(Address), results->ai_addr, results->ai_addrlen);


	freeaddrinfo(results);

	connect(tcpSocketIPV4->Get(), reinterpret_cast<const sockaddr*>(&Address), sizeof(Address));

	//trying string instead
	//char request[1024] = "GET / HTTP/1.1\r\n";
	std::string request = "GET / HTTP/1.1\r\n";
	//strcat_s(request, sizeof(request), origin);
	request += origin;
	//strcat_s(request, sizeof(request), "Connection: close\r\nUser-Agent:TestAgent\r\n\r\n");
	request += "Connection: close\r\nUser-Agent:TestAgent\r\n\r\n";

	send(tcpSocketIPV4->Get(), request.c_str(), request.size() + 1, 0);

	char buffer[1024]{};
	//work on recv here for larger responses than 1024 bytes
	recv(tcpSocketIPV4->Get(), buffer, sizeof(buffer), 0);

	return buffer;
}