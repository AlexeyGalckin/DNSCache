#include <gtest/gtest.h>
//
#include "library.h"
//
TEST(test_library, success)
{
	ASSERT_EQ(function(), 0);
}
