#pragma once
//
#include <string>
//
class DNSCache
{
public:
	explicit DNSCache(size_t max_size);
	//
	void update(const std::string& name, const std::string& ip);
	//
	std::string resolve(const std::string& name);
};