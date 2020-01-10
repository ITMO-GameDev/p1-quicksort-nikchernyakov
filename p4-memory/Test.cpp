#include "gtest/gtest.h"

#include "MemoryAllocator.h"

class MemoryAllocatorTest : public ::testing::Test
{
protected:

	MemoryAllocator allocator;

	void SetUp() override
	{
		allocator.init();
	}

	void TearDown() override
	{
		allocator.destroy();
	}
};

TEST_F(MemoryAllocatorTest, NullAllocation)
{
	void* p = allocator.alloc(0);

	ASSERT_EQ(p, nullptr);
}

TEST_F(MemoryAllocatorTest, FsaAllocationTest)
{
	void* p = allocator.alloc(sizeof(int));

	ASSERT_NE(p, nullptr);
	ASSERT_EQ((uintptr_t)p % 8, 0);
}

TEST_F(MemoryAllocatorTest, CoalesceAllocationTest)
{
	void* p = allocator.alloc(10000 * sizeof(int));

	ASSERT_NE(p, nullptr);
	ASSERT_EQ((uintptr_t)p % 8, 0);
}

TEST_F(MemoryAllocatorTest, LargeAllocationTest)
{
	void* p = allocator.alloc(11 * 1024 * 1024);

	ASSERT_NE(p, nullptr);
	ASSERT_EQ((uintptr_t)p % 8, 0);
}
