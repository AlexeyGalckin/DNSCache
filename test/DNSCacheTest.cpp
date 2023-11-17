#include <gtest/gtest.h>
//
#include "DNSCacheMutex.h"
//
using cache_t = DNSCacheMutex;
//
TEST(DNSCacheTestBasic, ZeroSizeNoEtries)
{
	auto cache = std::make_unique< cache_t >(0);
	//
	cache->update("HOST", "1.1.1.1");
	//
	ASSERT_TRUE(cache->resolve("HOST").empty());
}
//
TEST(DNSCacheTestBasic, SingleEmptyNoSuchEntry)
{
	auto cache = std::make_unique< cache_t >(1);
	//
	ASSERT_TRUE(cache->resolve("TEST").empty());
	ASSERT_TRUE(cache->resolve("").empty());
}
//
TEST(DNSCacheTestBasic, SingleNoSuchEntry)
{
	auto cache = std::make_unique< cache_t >(1);
	//
	cache->update("HOST", "1.1.1.1");
	//
	ASSERT_TRUE(cache->resolve("TEST").empty());
	ASSERT_TRUE(cache->resolve("1.1.1.1").empty());
}
//
TEST(DNSCacheTestBasic, SingleReturnsEntry)
{
	auto cache = std::make_unique< cache_t >(1);
	//
	cache->update("HOST", "1.1.1.1");
	//
	ASSERT_EQ(cache->resolve("HOST"), "1.1.1.1");
}
//
TEST(DNSCacheTestBasic, MultiReturnsEntry)
{
	auto cache = std::make_unique< cache_t >(3);
	//
	cache->update("HOST", "1.1.1.1");
	cache->update("TEST", "2.2.2.2");
	cache->update("REST", "3.3.3.3");
	//
	ASSERT_EQ(cache->resolve("HOST"), "1.1.1.1");
	ASSERT_EQ(cache->resolve("TEST"), "2.2.2.2");
	ASSERT_EQ(cache->resolve("REST"), "3.3.3.3");
}
//
TEST(DNSCacheTestLRU, SingleUpdateReplace)
{
	auto cache = std::make_unique< cache_t >(1);
	//
	cache->update("HOST", "1.1.1.1");
	cache->update("TEST", "2.2.2.2");
	//
	ASSERT_TRUE(cache->resolve("HOST").empty());
	ASSERT_EQ(cache->resolve("TEST"), "2.2.2.2");
}
//
TEST(DNSCacheTestLRU, MultiUpdateSingleReplace)
{
	auto cache = std::make_unique< cache_t >(2);
	//
	cache->update("HOST", "1.1.1.1");
	cache->update("TEST", "2.2.2.2");
	cache->update("REST", "3.3.3.3");
	//
	ASSERT_TRUE(cache->resolve("HOST").empty());
	//
	ASSERT_EQ(cache->resolve("TEST"), "2.2.2.2");
	ASSERT_EQ(cache->resolve("REST"), "3.3.3.3");
}
//
TEST(DNSCacheTestLRU, MultiUpdateMultiReplace)
{
	auto cache = std::make_unique< cache_t >(2);
	//
	cache->update("HOST", "1.1.1.1");
	cache->update("TEST", "2.2.2.2");
	cache->update("REST", "3.3.3.3");
	cache->update("BEST", "4.4.4.4");
	//
	ASSERT_TRUE(cache->resolve("HOST").empty());
	ASSERT_TRUE(cache->resolve("TEST").empty());
	//
	ASSERT_EQ(cache->resolve("REST"), "3.3.3.3");
	ASSERT_EQ(cache->resolve("BEST"), "4.4.4.4");
}
//
TEST(DNSCacheTestLRU, MultiResolveSingleReplace)
{
	auto cache = std::make_unique< cache_t >(2);
	//
	cache->update("HOST", "1.1.1.1");
	cache->update("TEST", "2.2.2.2");
	//
	EXPECT_EQ(cache->resolve("HOST"), "1.1.1.1");
	//
	cache->update("REST", "3.3.3.3");
	//
	ASSERT_TRUE(cache->resolve("TEST").empty());
	//
	ASSERT_EQ(cache->resolve("HOST"), "1.1.1.1");
	ASSERT_EQ(cache->resolve("REST"), "3.3.3.3");
}
//
TEST(DNSCacheTestLRU, MultiResolveUpdateMultiReplace)
{
	auto cache = std::make_unique< cache_t >(3);
	//
	cache->update("HOST", "1.1.1.1");
	cache->update("TEST", "2.2.2.2");
	cache->update("REST", "3.3.3.3");
	//
	EXPECT_EQ(cache->resolve("HOST"), "1.1.1.1");
	//
	cache->update("TEST", "2.2.2.3");
	//
	cache->update("BEST", "100.100.100.100");
	//
	ASSERT_TRUE(cache->resolve("REST").empty());
	//
	ASSERT_EQ(cache->resolve("HOST"), "1.1.1.1");
	ASSERT_EQ(cache->resolve("TEST"), "2.2.2.3");
	ASSERT_EQ(cache->resolve("BEST"), "100.100.100.100");
}