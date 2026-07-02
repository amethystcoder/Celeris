
#include "net/cleansocket.h"

CleanSocket::CleanSocket(int address_family, int sockettype, int protocol) {
	cleanSocket = socket(address_family, sockettype, protocol);
#if PLATFORM_WINDOWS
	if (cleanSocket == INVALID_SOCKET) {
		throw std::system_error(WSAGetLastError(), std::system_category());
	}
#else 
	if (cleanSocket == INVALID_SOCKET) {
		throw std::system_error(errno, std::system_category());
	}
#endif
}

SOCKET CleanSocket::Get() const noexcept {
	return cleanSocket;
}

CleanSocket::CleanSocket(CleanSocket&& rhs) noexcept :
	cleanSocket{ std::exchange(rhs.cleanSocket, INVALID_SOCKET) }
{}

CleanSocket& CleanSocket::operator = (CleanSocket&& rhs) noexcept {
	if (cleanSocket != INVALID_SOCKET) {
		std::ignore = closesocket(cleanSocket);
	}

	cleanSocket = std::exchange(rhs.cleanSocket, INVALID_SOCKET);

	return *this;
}

CleanSocket::~CleanSocket() noexcept {
	if (cleanSocket != INVALID_SOCKET) {
		std::ignore = closesocket(cleanSocket);
	}
}

