#include "DNSCacheMutex.h"
//
DNSCacheMutex::DNSCacheMutex(size_t max_size)
	: _size(max_size)
{
}
//
void DNSCacheMutex::update(const std::string& name, const std::string& ip)
{
    if (_size == 0)
        return;
    //
    auto lock = std::lock_guard(_mutex);
    //
    auto t = _cache.find(name);
    //
    if (t == _cache.end())
    {
        if (_cache.size() == _size)
        {
            _cache.erase(_lru.front().first);
            _lru.pop_front();
        }
        //
        auto t = _lru.insert(_lru.end(), { name, ip });
        _cache.insert({ t->first , t });
    }
    else
    {
        t->second->second = ip;
        _lru.splice(_lru.end(), _lru, t->second);
    }
}
//
std::string DNSCacheMutex::resolve(const std::string& name)
{
    auto lock = std::lock_guard(_mutex);
    //
    auto t = _cache.find(name);
    //
    if (t == _cache.end())
        return {};
    //
    _lru.splice(_lru.end(), _lru, t->second);
    //
    return t->second->second;
}
//
