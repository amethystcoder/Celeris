#include "core/RatelimitNode.h"
#include <iostream>
#include <ctime>

using namespace std::literals::chrono_literals;
//using namespace Celeris::Utilities;

// returns the current unix time in seconds
static uint64_t now_seconds() {
	return static_cast<uint64_t>(std::time(nullptr));
}

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
  // parse maxRequests into rate
  if (this->nodeAttributes.find("maxRequests") != this->nodeAttributes.end()) {
    try {
      this->rate = static_cast<unsigned short>(std::stoi(this->nodeAttributes["maxRequests"]));
    } catch (...) {
      this->rate = 100; // default if parsing fails
    }
  } else {
    this->rate = 100; // default
  }
  // parse the `per` window (e.g. "60s", "5m", "1h") into seconds
  if (this->nodeAttributes.find("per") != this->nodeAttributes.end()) {
    std::string per = this->nodeAttributes["per"];
    try {
      unsigned long value = std::stoul(per); // reads leading digits, ignores trailing unit
      char unit = per.empty() ? 's' : per.back();
      switch (unit) {
        case 'm': this->window_seconds = value * 60; break;    // minutes
        case 'h': this->window_seconds = value * 3600; break;  // hours
        case 's': default: this->window_seconds = value; break; // seconds
      }
    } catch (...) {
      this->window_seconds = 60; // default
    }
  } else {
    this->window_seconds = 60; // default
  }
	//quick setup for reusable attributes
	if (this->nodeAttributes.find("bktSize") != this->nodeAttributes.end()) {
		this->nodeAttributes["bktSize"];
	}
}

bool RateLimitNode::beginLimit() {
	return true;
}

// counts a request for the ip within the current fixed window.
// if the window has expired, it starts a fresh window with count = 1.
void RateLimitNode::addNewIpaddress(const std::string& ip_address)
{
	std::lock_guard<std::mutex> lock(this->attempts_mutex);
	uint64_t now = now_seconds();
	auto it = this->ip_attempts_map.find(ip_address);

	if (it == this->ip_attempts_map.end()) {
		// first time we've seen this ip: count = 1, window starts now
		this->ip_attempts_map[ip_address] = { 1, now };
		return;
	}

	// existing ip: has the current window expired?
	if (now - it->second.second >= this->window_seconds) {
		// window expired -> start a fresh window
		it->second.first = 1;
		it->second.second = now;
	} else {
		// still within the window -> increment the attempt count
		it->second.first += 1;
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

// returns true if the ip has exceeded `rate` requests within the current window
bool RateLimitNode::isRateLimited(const std::string& ip_address){
	std::lock_guard<std::mutex> lock(this->attempts_mutex);
	auto it = this->ip_attempts_map.find(ip_address);
	if (it == this->ip_attempts_map.end()) return false;

	uint64_t now = now_seconds();
	// if the window already expired, they're not limited
	// (the next addNewIpaddress call will reset the window)
	if (now - it->second.second >= this->window_seconds) return false;

	// within the window: limited once the count exceeds the allowed rate
	return it->second.first > this->rate;
}
