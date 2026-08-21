#ifndef CONNECTIONREQUEST_H
#define CONNECTIONREQUEST_H

#include "net/serversock.h"
#include "protocols/http/httprequest.h"
#include <mutex>

class ConnectionRequest
{
public:
    ConnectionRequest(const ConnectionRequest&) = delete;
    ConnectionRequest& operator=(const ConnectionRequest&) = delete;

    //access the single instance
	static ConnectionRequest& getInstance() noexcept
    {
        static ConnectionRequest instance;
        return instance;
    }

    //thread-safe setters
    void setSocket(SOCKET sock) noexcept {
        std::lock_guard<std::mutex> lock(mutex);
        this->socket = sock;
    }

    void setHeaders(const HTTPHeaderMap& headers) noexcept {
        std::lock_guard<std::mutex> lock(mutex);
        this->headers = headers;
    }

    void setContent(const std::string& content) noexcept {
        std::lock_guard<std::mutex> lock(mutex);
        this->content = content;
    }

    void setIpAddress() noexcept {
        std::lock_guard<std::mutex> lock(mutex);
        this->ipAddress = serverSock.getClientIp(socket);
    }

    //thread-safe getters
    SOCKET getSocket() const noexcept {
        std::lock_guard<std::mutex> lock(mutex);
        return this->socket;
    }

    HTTPHeaderMap getHeaders() const noexcept {
        std::lock_guard<std::mutex> lock(mutex);
        return this->headers;
    }

    std::string getContent() const noexcept {
        std::lock_guard<std::mutex> lock(mutex);
        return this->content;
    }

    std::string getIpAddress() const noexcept {
        std::lock_guard<std::mutex> lock(mutex);
        return this->ipAddress;
    }

	std::string getRoute() const noexcept {
		std::lock_guard<std::mutex> lock(mutex);
		return this->route;
	}

    void setRoute(const std::string& route) noexcept {
        std::lock_guard<std::mutex> lock(mutex);
        this->route = route;
    }

	std::string setRequestMethod(const std::string& method) noexcept {
		std::lock_guard<std::mutex> lock(mutex);
		this->requestMethod = method;
		return this->requestMethod;
	}

	std::string getRequestMethod() const noexcept {
		std::lock_guard<std::mutex> lock(mutex);
		return this->requestMethod;
	}

	//clear the request data
	void clear() noexcept {
		std::lock_guard<std::mutex> lock(mutex);
		ipAddress.clear();
		socket = INVALID_SOCKET;
		headers.clear();
		content.clear();
		route.clear();
	}

private:
    //need to create overloads for these as well as define what the copy, move, move assignment, and copy assignment should do here
    ConnectionRequest() = default;
    ~ConnectionRequest() = default;

    mutable std::mutex mutex;

    std::string ipAddress{ "" };
    SOCKET socket{ INVALID_SOCKET };
    HTTPHeaderMap headers;
    std::string route{ "" };
    std::string requestMethod{ "" };
    Celeris::ServerSocket serverSock{};
    std::string content{ "" };
};

#endif // !CONNECTIONREQUEST_H
