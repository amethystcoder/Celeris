#include "net/serversock.h"

Celeris::ServerSocket::ServerSocket()
{
}

Celeris::ServerSocket::~ServerSocket() noexcept
{
}


//ip_addr is the ip address of the server and looks like "127.0.0.1"
void Celeris::ServerSocket::listenforConnections(CleanSocket* tcpSocketIPV4, const char ip_addr[], u_int port) {

	sockaddr_in hint{};
	hint.sin_family = AF_INET;
	hint.sin_addr.s_addr = INADDR_ANY; 
	hint.sin_port = htons(port);

	//there is an issue with inet_pton here that i have to fix
	//inet_pton(AF_INET, ip_addr, &hint.sin_addr);

	int bind_result = bind(tcpSocketIPV4->Get(), reinterpret_cast<const sockaddr*>(&hint), sizeof(hint));

#if PLATFORM_WINDOWS
	if (bind_result == SOCKET_ERROR) {
		//try to prevent the use of throwing exceptions here
		throw std::system_error(WSAGetLastError(), std::system_category());
	}
#else
	if (bind_result == SOCKET_ERROR) {
		//try to prevent the use of throwing exceptions here
		throw std::system_error(errno, std::system_category());
	}
#endif

	//bind should be successful at this stage

	int listen_result = listen(tcpSocketIPV4->Get(), SOMAXCONN);

#if PLATFORM_WINDOWS
	if (listen_result == SOCKET_ERROR) {
		//try to prevent the use of throwing exceptions here
		throw std::system_error(WSAGetLastError(), std::system_category());
	}
#else
	if (listen_result < 0) {
		//fix up posix errors properly
		throw std::system_error(errno, std::system_category());
	}
#endif
}

SOCKET Celeris::ServerSocket::acceptConnection(CleanSocket* tcpSocketIPV4) {
	sockaddr_in client{};
#if PLATFORM_WINDOWS
	int clientSize = sizeof(client);
#else
	socklen_t clientSize = sizeof(client);
#endif

#if PLATFORM_WINDOWS
	SOCKET clientSocket = accept(tcpSocketIPV4->Get(), reinterpret_cast<sockaddr*>(&client), &clientSize);
	if (clientSocket == INVALID_SOCKET) {
		throw std::system_error(WSAGetLastError(), std::system_category());
	}
#else
	//posix systems 
	SOCKET clientSocket = accept(tcpSocketIPV4->Get(), reinterpret_cast<sockaddr*>(&client), &clientSize);
	if (clientSocket < 0) {
		//determine error type
		perror("Failed to send");
		throw std::system_error(errno, std::system_category());
	}
#endif // 0

	//socket information might be needed by other components
	return clientSocket;
}

std::string Celeris::ServerSocket::receiveData(SOCKET clientSocket) { //we do not use a clean socket here?
	char buf[4096];

#if PLATFORM_WINDOWS
	ZeroMemory(buf, (sizeof(char) * 4096));
#else
	//bzero(buf, 4096);
	memset(buf, 0, (sizeof(char) * 4096));
#endif

	int bytesReceived = recv(clientSocket, buf, 4096, 0);
#if PLATFORM_WINDOWS
	if (bytesReceived == SOCKET_ERROR) {
		throw std::system_error(WSAGetLastError(), std::system_category());
	}
#else
	//posix systems 
	if (bytesReceived < 0) {
		//determine error type
		perror("Failed to send");
		throw std::system_error(errno, std::system_category());
	}
#endif // 0

	return std::string(buf, 0, bytesReceived);
}

void Celeris::ServerSocket::sendData(SOCKET& clientSocket, const char data[]) {
	int sendData = send(clientSocket, data, static_cast<int>(strlen(data)) + 1, 0);//add 1 to the length to include the null terminator
#if PLATFORM_WINDOWS
	if (sendData == SOCKET_ERROR) {
		throw std::system_error(WSAGetLastError(), std::system_category());
	}
	//Sleep(1000); //check for why this was included
	shutdown(clientSocket, SD_SEND);  //shutdown the sending side of the socket
#else
	//posix systems 
	if (sendData < 0) {
		//determine error type
		perror("Failed to send");
	}
	//Sleep(1000); //check for why this was included
	shutdown(clientSocket, SHUT_RDWR);  //shutdown the sending side of the socket
#endif // 0
}

std::string Celeris::ServerSocket::getClientIp(SOCKET clientSocket) {
	sockaddr_in clientAddr;
#if PLATFORM_WINDOWS
	int addrLen = sizeof(clientAddr);
#else
	socklen_t addrLen = sizeof(clientAddr);
#endif
	if (getpeername(clientSocket, (sockaddr*)&clientAddr, &addrLen) == 0) {
		char ipStr[INET_ADDRSTRLEN];
#if PLATFORM_WINDOWS
		Celeris::SocketImpl::inet_ntop(AF_INET, &clientAddr.sin_addr, ipStr, sizeof(ipStr));
#elif PLATFORM_POSIX
		inet_ntop(AF_INET, &clientAddr.sin_addr, ipStr, sizeof(ipStr));
#endif
		return std::string(ipStr);
	}
	return "";
}

