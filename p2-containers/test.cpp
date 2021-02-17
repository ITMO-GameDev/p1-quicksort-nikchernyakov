#include "gtest/gtest.h"
#include "array.h"

template <typename T>
void fill_array_content(Array<T>& array_class, const T* array, const int size);

template <typename T>
bool compare_array_content(const Array<T>& array_class, const T* array, const int size);

template <typename T>
void print_array_content(const Array<T>& array_class);

TEST(ArrayCreatingTest, EmptyDefaultTest)
{
	Array<int> array;
	ASSERT_EQ(array.size(), 0);
	ASSERT_EQ(array.capacity(), 8);
}

TEST(ArrayCreatingTest, EmptyWithCertainCapacityTest)
{
	Array<int> array(2);
	ASSERT_EQ(array.size(), 0);
	ASSERT_EQ(array.capacity(), 2);
}

TEST(ArrayCreatingTest, FillArrayWithDefaultCapacityTest)
{
	const auto content_size = 3;
	const int* array_content = new int[content_size]{ 2, 3, 5 };
	Array<int> array;
	fill_array_content(array, array_content, content_size);

	ASSERT_EQ(array.size(), content_size);
	ASSERT_EQ(array.capacity(), 8);
	ASSERT_TRUE(compare_array_content(array, array_content, content_size));
}

TEST(ArrayCreatingTest, FillAndOverloadCapacityTest)
{
	const auto content_size = 9;
	const int* array_content = new int[content_size] { 2, 3, 5, 4, 1, 6, 7, 9, 10 };
	Array<int> array;
	fill_array_content(array, array_content, content_size);

	ASSERT_EQ(array.size(), content_size);
	ASSERT_EQ(array.capacity(), 16);
	ASSERT_TRUE(compare_array_content(array, array_content, content_size));
}

TEST(ArrayEditingTest, InsertInCertainIndexTest)
{
	const auto content_size = 4;
	const int* array_content = new int[content_size] { 2, 3, 5, 4 };
	Array<int> array;
	fill_array_content(array, array_content, content_size);

	array.insert(2, 10);
	const auto new_content_size = 5;
	const int* new_array_content = new int[new_content_size] { 2, 3, 10, 5, 4 };
	ASSERT_EQ(array.size(), new_content_size);
	ASSERT_TRUE(compare_array_content(array, new_array_content, new_content_size));
}

TEST(ArrayEditingTest, RemoveCertainElementTest)
{
	const auto content_size = 4;
	const int* array_content = new int[content_size] { 2, 3, 5, 4 };
	Array<int> array;
	fill_array_content(array, array_content, content_size);

	array.remove(1);
	const auto new_content_size = 3;
	const int* new_array_content = new int[new_content_size] { 2, 5, 4 };
	ASSERT_EQ(array.size(), new_content_size);
	ASSERT_TRUE(compare_array_content(array, new_array_content, new_content_size));
}

TEST(IteratorTest, ForwardMovingTest)
{
	Array<int> array;
	auto iterator = array.iterator();

	iterator.insert(0);
	iterator.insert(1);
	iterator.insert(2);
	iterator.insert(3);

	// Check correctness
	for (int i = 3; iterator.has_next(); iterator.next(), i--)
	{
		EXPECT_EQ(iterator.get(), i);
	}
}

TEST(IteratorTest, BackwardMovingTest)
{
	Array<int> array;
	auto iterator = array.iterator();

	iterator.insert(0);
	iterator.insert(1);
	iterator.insert(2);
	iterator.insert(3);

	// Check correctness
	iterator.to_index(3);
	for (int i = 0; iterator.has_prev(); iterator.prev(), i++)
	{
		EXPECT_EQ(iterator.get(), i);
	}
}

TEST(IteratorTest, SetAndRemoveTest)
{
	Array<int> array;
	auto iterator = array.iterator();

	array.insert(1);
	array.insert(5); // Will be changed
	array.insert(3);
	array.insert(9); // Will be removed
	array.insert(4);

	iterator.to_index(3);
	iterator.remove();

	iterator.to_index(1);
	iterator.set(2);

	// Check correctness
	iterator.to_index(0);
	for (int i = 1; iterator.has_next(); iterator.next(), i++)
	{
		EXPECT_EQ(iterator.get(), i);
	}
}

template <typename T>
void fill_array_content(Array<T>& array_class, const T* array, const int size) {
	for (int i = 0; i < size; i++) {
		array_class.insert(array[i]);
	}
}

template <typename T>
bool compare_array_content(const Array<T>& array_class, const T* array, const int size) {
	for (int i = 0; i < size; i++) {
		if (array_class[i] != array[i]) {
			return false;
		}
	}
	return true;
}

template <typename T>
void print_array_content(const Array<T>& array_class) {
	for (int i = 0; i < array_class.size(); i++) {
		std::cout << array_class[i] << " ";
	}
	std::cout << "\n";
}