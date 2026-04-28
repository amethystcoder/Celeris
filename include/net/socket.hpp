//
// Amethystcoder
// Web Server Project
// 
// 2024

#ifndef SOCKET_IMPL

#define SOCKET_IMPL

#define Amthsocket

#define SOCKETCONNECTIONSUCCESS 0

#include <iostream>
#include <stdexcept>
#include <system_error>
#include <utility>
#include "platform/platform.h"


namespace AmthSocket {


	class SocketImpl
	{
	public:
		SocketImpl();
		~SocketImpl() noexcept;

		SocketImpl(const SocketImpl&) = delete;
		SocketImpl& operator = (const SocketImpl&) = delete;

		SocketImpl(SocketImpl&& socklib) noexcept :
			socketInitialized{ std::exchange(socklib.socketInitialized,false)}
#if PLATFORM_WINDOWS
			,WSAdata{ socklib.WSAdata }
#endif
		{}


		SocketImpl& operator = (SocketImpl&& rhs) noexcept {
			socketInitialized = std::exchange(rhs.socketInitialized, false);
#if PLATFORM_WINDOWS
			WSAdata = rhs.WSAdata;
#endif
			return *this;
		}

		explicit SocketImpl(std::nothrow_t);


		inline bool isInitialized() const noexcept;

		#if PLATFORM_WINDOWS
		inline const WSADATA& getWSAData() const noexcept;
#else
		inline void getWSAData() const noexcept {}
#endif

		void handleStartupError(int errorcode);
	private:
#if PLATFORM_WINDOWS
		WSADATA WSAdata{};
#endif
		bool socketInitialized = false;
	};
}

#endif // !SOCKET_IMPL