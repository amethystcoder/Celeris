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


namespace Celeris {


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
		//some windows socket implementations and libraries do not have this
		//so this would be included for windows
		static int inet_pton(int af, const char* src, void* dst);
		static const char* inet_ntop(int af, const void* src, char* dst, socklen_t size);
#else
		//in the case that there is code somewhere that relies on this method.
		//there should not be, but in the case that there is.
		inline void getWSAData() const noexcept;
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