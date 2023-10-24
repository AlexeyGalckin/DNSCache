#include <gtest/gtest.h>
//
#include "DNSCache.h"
//
TEST(DNSCacheTestBasic, EmptyNoSuchEntry)
{
	DNSCache cache(1);
	//
	ASSERT_TRUE(cache.resolve("TEST").empty());
	ASSERT_TRUE(cache.resolve("").empty());
}
//
TEST(DNSCacheTestBasic, SingleNoSuchEntry)
{
	DNSCache cache(1);
	//
	cache.update("HOST", "1.1.1.1");
	//
	ASSERT_TRUE(cache.resolve("TEST").empty());
	ASSERT_TRUE(cache.resolve("1.1.1.1").empty());
}
//
TEST(DNSCacheTestBasic, SingleReturnsEntry)
{
	DNSCache cache(1);
	//
	cache.update("HOST", "1.1.1.1");
	//
	ASSERT_EQ(cache.resolve("HOST"), "1.1.1.1");
}
//
TEST(DNSCacheTestBasic, MultiReturnsEntry)
{
	DNSCache cache(3);
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
	DNSCache cache(1);
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
	DNSCache cache(2);
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
	DNSCache cache(2);
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
	DNSCache cache(2);
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