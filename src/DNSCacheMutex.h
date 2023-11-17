#pragma once
//
#include "DNSCache.h"
//
#include <string>
#include <list>
#include <unordered_map>
#include <mutex>
#include <memory>
//
class DNSCacheMutex : public DNSCache
{
public:
	//
	explicit DNSCacheMutex(size_t max_size);
	//
	virtual void update(const std::string& name, const std::string& ip) override;
	//
	virtual std::string resolve(const std::string& name) override;
private:
	//
	using entry_t = std::pair< std::string, std::string >;
	using list_t  = std::list< entry_t >;
	using cache_t = std::unordered_map< std::string_view, list_t::iterator >;
	using mutex_t = std::mutex;
	//
	list_t  _lru;
	cache_t _cache;
	mutex_t _mutex;
	//
	const size_t _size;
};