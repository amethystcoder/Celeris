#include "net/socket.hpp"
#include <cstring>

Celeris::SocketImpl::SocketImpl()
{
#if PLATFORM_WINDOWS
	const int startupResult = WSAStartup(WINSOCK_VERSION, &WSAdata);
	if (startupResult != 0) {
		//Throw an exception
		throw std::system_error(startupResult, std::system_category());
	}
#elif PLATFORM_POSIX
	// POSIX systems don't need WSAStartup
#endif
	socketInitialized = true;
}

Celeris::SocketImpl::~SocketImpl() noexcept
{
	if (socketInitialized) {
#if PLATFORM_WINDOWS
		std::ignore = WSACleanup();
		/* if (cleanupResult != 0) {
			int WSAerror = WSAGetLastError();
		} */
#elif PLATFORM_POSIX
		// POSIX systems don't need WSACleanup
#endif
	}
}

void Celeris::SocketImpl::handleStartupError(int errorcode) {
	std::cerr << "Error: " << errorcode << std::endl;
#if PLATFORM_WINDOWS
	//handle each category of error
	switch (errorcode)
	{
	case WSASYSNOTREADY:
		std::cerr << "The underlying network subsystem is not ready for network communication." << std::endl;
		break;
	case WSAVERNOTSUPPORTED:
		std::cerr << "The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation." << std::endl;
		break;
	case WSAEINPROGRESS:
		std::cerr << "A blocking Windows Sockets 1.1 operation is in progress." << std::endl;
		break;
	case WSAEPROCLIM:
		std::cerr << "A limit on the number of tasks supported by the Windows Sockets implementation has been reached." << std::endl;
		break;
	case WSAEFAULT:
		std::cerr << "The lpWSAData parameter is not a valid pointer." << std::endl;
		break;
	case WSANOTINITIALISED:
		std::cerr << "A successful WSAStartup call must occur before using this function." << std::endl;
		break;
	default:
		break;
	}
#elif PLATFORM_POSIX
	// POSIX error handling
	switch (errorcode) {
	case EACCES:
		std::cerr << "Permission denied." << std::endl;
		break;
	case EADDRINUSE:
		std::cerr << "Address already in use." << std::endl;
		break;
	case ECONNREFUSED:
		std::cerr << "Connection refused." << std::endl;
		break;
	default:
		std::cerr << "Socket error: " << strerror(errorcode) << std::endl;
		break;
	}
#endif
	//still deciding whether it is a good idea to throw an exception here or not
	//throw std::system_error(errorcode, std::system_category());
}


Celeris::SocketImpl::SocketImpl(std::nothrow_t) {
#if PLATFORM_WINDOWS
	const int startupResult = WSAStartup(WINSOCK_VERSION, &WSAdata);
	if (startupResult != SOCKETCONNECTIONSUCCESS) {
		//TODO: Handle the error here 
		handleStartupError(WSAGetLastError());
		return;
	}
#elif PLATFORM_POSIX
	// POSIX systems don't need WSAStartup
#endif
	socketInitialized = true;
}


inline bool Celeris::SocketImpl::isInitialized() const noexcept {
	return socketInitialized;
}

#if PLATFORM_WINDOWS
inline const WSADATA& Celeris::SocketImpl::getWSAData() const noexcept {
	return WSAdata;
}
#else
inline void Celeris::SocketImpl::getWSAData() const noexcept {
	// No-op on POSIX systems
}
#endif

//utility functions for Windows in the case that they do not already exist
#if PLATFORM_WINDOWS
int Celeris::SocketImpl::inet_pton(int af, const char* src, void* dst)
{
	struct sockaddr_storage ss;
	int size = sizeof(ss);
	char src_copy[INET6_ADDRSTRLEN + 1];

	ZeroMemory(&ss, sizeof(ss));

	strncpy_s(src_copy, src, INET6_ADDRSTRLEN + 1);
	src_copy[INET6_ADDRSTRLEN] = 0;

	if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr*)&ss, &size) == 0) {
		switch (af) {
		case AF_INET:
			*(struct in_addr*)dst = ((struct sockaddr_in*)&ss)->sin_addr;
			return 1;
		case AF_INET6:
			*(struct in6_addr*)dst = ((struct sockaddr_in6*)&ss)->sin6_addr;
			return 1;
		}
	}
	return 0;
}

const char* Celeris::SocketImpl::inet_ntop(int af, const void* src, char* dst, socklen_t size)
{
	struct sockaddr_storage ss;
	unsigned long s = size;

	ZeroMemory(&ss, sizeof(ss));
	ss.ss_family = af;

	switch (af) {
	case AF_INET:
		((struct sockaddr_in*)&ss)->sin_addr = *(struct in_addr*)src;
		break;
	case AF_INET6:
		((struct sockaddr_in6*)&ss)->sin6_addr = *(struct in6_addr*)src;
		break;
	default:
		return NULL;
	}

	return (WSAAddressToString((struct sockaddr*)&ss, sizeof(ss), NULL, dst, &s) == 0) ?
		dst : NULL;
}
#endif