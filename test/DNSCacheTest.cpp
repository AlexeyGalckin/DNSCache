#include <gtest/gtest.h>
//
#include "DNSCache.h"
//
TEST(DNSCacheTestBasic, ZeroSizeNoEtries)
{
	DNSCache::initialize(0);
	auto& cache = DNSCache::instance();
	//
	cache.update("HOST", "1.1.1.1");
	//
	ASSERT_TRUE(cache.resolve("HOST").empty());
}
//
TEST(DNSCacheTestBasic, SingleEmptyNoSuchEntry)
{
	DNSCache::initialize(1);
	auto& cache = DNSCache::instance();
	//
	ASSERT_TRUE(cache.resolve("TEST").empty());
	ASSERT_TRUE(cache.resolve("").empty());
}
//
TEST(DNSCacheTestBasic, SingleNoSuchEntry)
{
	DNSCache::initialize(1);
	auto& cache = DNSCache::instance();
	//
	cache.update("HOST", "1.1.1.1");
	//
	ASSERT_TRUE(cache.resolve("TEST").empty());
	ASSERT_TRUE(cache.resolve("1.1.1.1").empty());
}
//
TEST(DNSCacheTestBasic, SingleReturnsEntry)
{
	DNSCache::initialize(1);
	auto& cache = DNSCache::instance();
	//
	cache.update("HOST", "1.1.1.1");
	//
	ASSERT_EQ(cache.resolve("HOST"), "1.1.1.1");
}
//
TEST(DNSCacheTestBasic, MultiReturnsEntry)
{
	DNSCache::initialize(3);
	auto& cache = DNSCache::instance();
	//
	cache.update("HOST", "1.1.1.1");
	cache.update("TEST", "2.2.2.2");
	cache.update("REST", "3.3.3.3");
	//
	ASSERT_EQ(cache.resolve("HOST"), "1.1.1.1");
	ASSERT_EQ(cache.resolve("TEST"), "2.2.2.2");
	ASSERT_EQ(cache.resolve("REST"), "3.3.3.3");
}
//
TEST(DNSCacheTestLRU, SingleUpdateReplace)
{
	DNSCache::initialize(1);
	auto& cache = DNSCache::instance();
	//
	cache.update("HOST", "1.1.1.1");
	cache.update("TEST", "2.2.2.2");
	//
	ASSERT_TRUE(cache.resolve("HOST").empty());
	ASSERT_EQ(cache.resolve("TEST"), "2.2.2.2");
}
//
TEST(DNSCacheTestLRU, MultiUpdateSingleReplace)
{
	DNSCache::initialize(2);
	auto& cache = DNSCache::instance();
	//
	cache.update("HOST", "1.1.1.1");
	cache.update("TEST", "2.2.2.2");
	cache.update("REST", "3.3.3.3");
	//
	ASSERT_TRUE(cache.resolve("HOST").empty());
	//
	ASSERT_EQ(cache.resolve("TEST"), "2.2.2.2");
	ASSERT_EQ(cache.resolve("REST"), "3.3.3.3");
}
//
TEST(DNSCacheTestLRU, MultiUpdateMultiReplace)
{
	DNSCache::initialize(2);
	auto& cache = DNSCache::instance();
	//
	cache.update("HOST", "1.1.1.1");
	cache.update("TEST", "2.2.2.2");
	cache.update("REST", "3.3.3.3");
	cache.update("BEST", "4.4.4.4");
	//
	ASSERT_TRUE(cache.resolve("HOST").empty());
	ASSERT_TRUE(cache.resolve("TEST").empty());
	//
	ASSERT_EQ(cache.resolve("REST"), "3.3.3.3");
	ASSERT_EQ(cache.resolve("BEST"), "4.4.4.4");
}
//
TEST(DNSCacheTestLRU, MultiResolveSingleReplace)
{
	DNSCache::initialize(2);
	auto& cache = DNSCache::instance();
	//
	cache.update("HOST", "1.1.1.1");
	cache.update("TEST", "2.2.2.2");
	//
	EXPECT_EQ(cache.resolve("HOST"), "1.1.1.1");
	//
	cache.update("REST", "3.3.3.3");
	//
	ASSERT_TRUE(cache.resolve("TEST").empty());
	//
	ASSERT_EQ(cache.resolve("HOST"), "1.1.1.1");
	ASSERT_EQ(cache.resolve("REST"), "3.3.3.3");
}
//
TEST(DNSCacheTestLRU, MultiResolveUpdateMultiReplace)
{
	DNSCache::initialize(3);
	auto& cache = DNSCache::instance();
	//
	cache.update("HOST", "1.1.1.1");
	cache.update("TEST", "2.2.2.2");
	cache.update("REST", "3.3.3.3");
	//
	EXPECT_EQ(cache.resolve("HOST"), "1.1.1.1");
	//
	cache.update("TEST", "2.2.2.3");
	//
	cache.update("BEST", "100.100.100.100");
	//
	ASSERT_TRUE(cache.resolve("REST").empty());
	//
	ASSERT_EQ(cache.resolve("HOST"), "1.1.1.1");
	ASSERT_EQ(cache.resolve("TEST"), "2.2.2.3");
	ASSERT_EQ(cache.resolve("BEST"), "100.100.100.100");
}