#ifndef RATELIMITNODE_H
#define RATELIMITNODE_H

#include <map>
#include <ctime>
#include <chrono>
#include <thread>
#include "../ast/ast.h"
#include "../ast/ast_manager.h"
#include <mutex>

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
	unsigned long window_seconds{60}; //fixed-window length in seconds, parsed from `per`

	std::mutex attempts_mutex;

	//ipaddress => <attempts, last reset time>
	std::map<std::string, std::pair<uint16_t,uint64_t>> ip_attempts_map;

	std::vector<RawDependency*> rawDependencies = {};
};

#endif // !RATELIMITNODE_H