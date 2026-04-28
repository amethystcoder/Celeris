#ifndef PLATFORM_H
#define PLATFORM_H

// Platform detection macros
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS 1
    #define PLATFORM_POSIX 0
#elif defined(__linux__)
    #define PLATFORM_LINUX 1
    #define PLATFORM_POSIX 1
#elif defined(__APPLE__)
    #define PLATFORM_MACOS 1
    #define PLATFORM_POSIX 1
#elif defined(__unix__)
    #define PLATFORM_UNIX 1
    #define PLATFORM_POSIX 1
#else
    #error "Unsupported platform"
#endif

// Socket platform detection
#if PLATFORM_WINDOWS
    #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x501
    #endif
    #include <WinSock2.h>
    #include <WS2tcpip.h>
    #include <windows.h>
    #pragma comment(lib, "Ws2_32.lib")
#elif PLATFORM_POSIX
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <netdb.h>
    // Define Windows-compatible types for POSIX
    typedef int SOCKET;
    const int INVALID_SOCKET = -1;
    const int SOCKET_ERROR = -1;
    #define closesocket close
    #define WSAGetLastError() (errno)
    #define WSAStartup(a, b) (0)
    #define WSACleanup() (0)
    typedef struct {
        int dummy;
    } WSADATA;
#endif


// Common utilities
#if PLATFORM_WINDOWS
    #define SleepMs(ms) Sleep(ms)
#elif PLATFORM_POSIX
    #define SleepMs(ms) usleep((ms) * 1000)
#endif

#endif // PLATFORM_H
