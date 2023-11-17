#pragma once
//
#include <string>
//
class DNSCache
{
public:
	virtual void update(const std::string& name, const std::string& ip) = 0;
	//
	virtual std::string resolve(const std::string& name) = 0;
	//
	virtual ~DNSCache() = default;
};