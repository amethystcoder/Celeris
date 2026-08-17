#include "core/RatelimitNode.h"
#include <iostream>

using namespace std::literals::chrono_literals;
using namespace Celeris::Utilities;

RateLimitNode::RateLimitNode()
	: rate{ 0 }
{
}

RateLimitNode::~RateLimitNode()
{
}

void RateLimitNode::registernode(const std::string& name, const std::string& attributes, std::string& content)
{
	//sanity checks here please 
	addTagName(name, this);
	setNodeAttributes(ASTManager::parseattributes(attributes), this);
	ASTManager::addNodeChildrenFromContent(content, this);
	//quick setup for reusable attributes
	if (this->nodeAttributes.find("bktSize") != this->nodeAttributes.end()) {
		this->nodeAttributes["bktSize"];
	}
}

bool RateLimitNode::beginLimit() {
	return true;
}

// adds one to the ip_address if it is already in memory, or creates a new std::pair with the ip_address and sets it to zero
void RateLimitNode::addNewIpaddress(const std::string& ip_address)
{
	//check that the ip address is not already in the map
	if (this->ip_attempts_map.find(ip_address) != this->ip_attempts_map.end()) {
		this->ip_attempts_map[ip_address].first += 1;
	}
	else {
		this->ip_attempts_map[ip_address].first = 1;
	}
}

void RateLimitNode::removeIpaddress(std::string& ip_address)
{
	//remove the ip address from the map
	this->ip_attempts_map.erase(this->ip_attempts_map.find(ip_address));
}

int RateLimitNode::getIpAttempts(const std::string& ip_address)
{
	//check if the ip address is in the map
	if (this->ip_attempts_map.find(ip_address) != this->ip_attempts_map.end()) {
		return this->ip_attempts_map[ip_address].first;
	}
	return 0;
}

//TODO: Check this function properly
bool RateLimitNode::isRateLimited(const std::string& ip_address){
	//check that `maxRequests` is exists as an attribute ... else use 100
	if(this->nodeAttributes.find("maxRequests") != this->nodeAttributes.end())
	if (this->ip_attempts_map.find(ip_address) != this->ip_attempts_map.end()) {
		//we need to keep track of the time and check if the time is within the rate limit
		if (this->ip_attempts_map[ip_address].second >= this->rate) {
			return true;
		}
	}
	return false;
}




//////////////////////////////////////////////////////////////////
// Implementations for ratelimiting types determined by the `type` attribute
// 
//////////////////////////////////////////////////////////////////
bool LeakyBucket::add_connection_to_bucket(const ConnectionRequest& connection) {
	if (requests.size() == max_size) return false;

	requests.push_back(connection);
	return true;
}