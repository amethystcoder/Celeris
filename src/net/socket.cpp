#include "net/socket.hpp"
#include <cstring>

AmthSocket::SocketImpl::SocketImpl()
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

AmthSocket::SocketImpl::~SocketImpl() noexcept
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

void AmthSocket::SocketImpl::handleStartupError(int errorcode) {
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


AmthSocket::SocketImpl::SocketImpl(std::nothrow_t) {
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


inline bool AmthSocket::SocketImpl::isInitialized() const noexcept {
	return socketInitialized;
}

#if PLATFORM_WINDOWS
inline const WSADATA& AmthSocket::SocketImpl::getWSAData() const noexcept {
	return WSAdata;
}
#elif PLATFORM_POSIX
inline void AmthSocket::SocketImpl::getWSAData() const noexcept {
	// No-op on POSIX systems
}
#endif