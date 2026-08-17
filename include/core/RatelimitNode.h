#ifndef RATELIMITNODE_H
#define RATELIMITNODE_H

#include <map>
#include <ctime>
#include <chrono>
#include <thread>
#include "../ast/ast.h"
#include "../ast/ast_manager.h"
#include <mutex>
#include <deque>

namespace Celeris::Utilities {

	using Request = unsigned long;
	struct LeakyBucket {

		//determine the type of info the token bucket would receive
		//a leaky bucket would have a buffer
		//a buffer of 
		LeakyBucket(const int max_size)
			: requests({}), max_size(max_size)
		{}

		//todo: connection request must be in the namespace
		/////////////////////////////////
		// creates a unique identifier for a request
		long assign_request(ConnectionRequest connection);

		//adds a connection to the bucket. returns false if addition failed
		bool add_connection_to_bucket(const ConnectionRequest& connection);

	protected:
		std::deque<ConnectionRequest> requests;

		int max_size;
	};

	struct TokenBucket {
		TokenBucket(const unsigned int size)
			: size(size)
		{
		}

		int size;
	};

	struct SlidingWindow {

	};
};

class RateLimitNode : public ASTreeNode
{
public:
	RateLimitNode();
	~RateLimitNode();

	void registernode(const std::string& name, const std::string& attributes, std::string& content);

	bool beginLimit();

	void addNewIpaddress(const std::string& ip_address);

	void removeIpaddress(std::string& ip_address);

	bool isRateLimited(const std::string& ip_address);

	int getIpAttempts(const std::string& ip_address);

	bool leaky_bucket();

	bool token_bucket();

	bool sliding_window();

	bool fixed_window();

private:

	struct RTConnection {};

	std::string time;
	unsigned short rate;
	unsigned long bf_size{0};

	std::mutex attempts_mutex;

	//ipaddress => <attempts, last reset time>
	std::map<std::string, std::pair<uint16_t,uint64_t>> ip_attempts_map;

	std::vector<RawDependency*> rawDependencies = {};
};

#endif // !RATELIMITNODE_H