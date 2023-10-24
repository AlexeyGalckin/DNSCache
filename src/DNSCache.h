#pragma once
//
#include <string>
#include <list>
#include <unordered_map>
//
class DNSCache
{
public:
	explicit DNSCache(size_t max_size);
	//
	void update(const std::string& name, const std::string& ip);
	//
	std::string resolve(const std::string& name);
private:
	using entry_t = std::pair< std::string, std::string >;
	using list_t  = std::list< entry_t >;
	using cache_t = std::unordered_map< std::string_view, list_t::iterator >;
	//
	list_t  _lru;
	cache_t _cache;
	//
	size_t _size;
};