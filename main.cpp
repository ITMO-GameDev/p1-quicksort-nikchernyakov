#include "gtest/gtest.h"

int main()
{
	// Don't understand how to configure it
	//::testing::GTEST_FLAG(filter) = "*SortTest*:*MedianTest*:*PerformanceTest*";
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}
