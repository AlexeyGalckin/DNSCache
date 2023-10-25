#pragma once
//
#include <string>
#include <list>
#include <unordered_map>
#include <mutex>
#include <memory>
//
class DNSCache
{
public:
	static void initialize(size_t max_size);
	static DNSCache& instance() noexcept;
	//
	void update(const std::string& name, const std::string& ip);
	//
	std::string resolve(const std::string& name);
private:
	DNSCache(const DNSCache&) = delete;
	DNSCache& operator=(const DNSCache&) = delete;
	//
	explicit DNSCache(size_t max_size);
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
	//
	static std::unique_ptr< DNSCache > _inst;
};