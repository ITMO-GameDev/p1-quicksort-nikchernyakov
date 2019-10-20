#include "gtest/gtest.h"
#include "sort.h"
#include <windows.h>

const auto intComparatorLambda = [](int a, int b) { return a > b; };
const auto quickSortMethodLambda = [](int* first, int* last) { sort(first, last, intComparatorLambda); };
const auto insertSortMethodLambda = [](int* first, int* last) { insertSort(first, last, intComparatorLambda); };

/* Utils methods for testing */
int* randomIntArray(int length, int bound = 200);

template<typename T, typename SortMethod>
double testArraySort(T* array, int length, SortMethod sortMethod,
	bool performanceCalculation = false, bool debugPrintArray = false);

/* Timer */
void startCounter();
double getCounter();

/* 
	Sort methods tests 
*/
TEST(SortTest, UnsortedArrayTest)
{
	int array[] = { 10, 5, 3, 9, 6, 1, 8, 0 };
	const int length = std::size(array);
	int copiedArray[length]; 
	std::copy(std::begin(array), std::end(array), std::begin(copiedArray));
	
	testArraySort(array, length, quickSortMethodLambda);
	testArraySort(copiedArray, length, insertSortMethodLambda);
}

TEST(SortTest, DescSortedArrayTest)
{
	int array[] = { 10, 9, 5, 4, 3, 2, 1, 0 };
	const int length = std::size(array);
	int copiedArray[length];
	std::copy(std::begin(array), std::end(array), std::begin(copiedArray));

	testArraySort(array, length, quickSortMethodLambda);
	testArraySort(copiedArray, length, insertSortMethodLambda);
}

TEST(SortTest, SortedArrayTest)
{
	int array[] = { 0, 1, 2, 4, 5, 6, 8, 10 };
	const int length = std::size(array);
	int copiedArray[length];
	std::copy(std::begin(array), std::end(array), std::begin(copiedArray));

	testArraySort(array, length, quickSortMethodLambda);
	testArraySort(copiedArray, length, insertSortMethodLambda);
}

// Test case where in partition method left bound reach pivot
TEST(SortTest, LeftBoundTest)
{
	int array[] = { 9, 5, 8, 1, 6, 4, 2, 7, 3 };
	const int length = std::size(array);
	int copiedArray[length];
	std::copy(std::begin(array), std::end(array), std::begin(copiedArray));

	testArraySort(array, length, quickSortMethodLambda);
	testArraySort(copiedArray, length, insertSortMethodLambda);
}

// Test case where in partition method right bound reach pivot
TEST(SortTest, RightBoundTest)
{
	int array[] = { 3, 5, 1, 7, 4, 6, 8, 9, 2 };
	const int length = std::size(array);
	int copiedArray[length];
	std::copy(std::begin(array), std::end(array), std::begin(copiedArray));

	testArraySort(array, length, quickSortMethodLambda);
	testArraySort(copiedArray, length, insertSortMethodLambda);
}

// Test case where in partition method bound moves further than pivot
TEST(SortTest, PivotOvercomeTest)
{
	int array[] = { 6, 4, 1, 9, 2, 6, 8, 7, 5 };
	const int length = std::size(array);
	int copiedArray[length];
	std::copy(std::begin(array), std::end(array), std::begin(copiedArray));

	testArraySort(array, length, quickSortMethodLambda);
	testArraySort(copiedArray, length, insertSortMethodLambda);
}


TEST(SortTest, OneElementArrayTest)
{
	int array[] = { 10 };
	const int length = std::size(array);
	int copiedArray[length];
	std::copy(std::begin(array), std::end(array), std::begin(copiedArray));

	testArraySort(array, length, quickSortMethodLambda);
	testArraySort(copiedArray, length, insertSortMethodLambda);
}

TEST(SortTest, EmptyArrayTest)
{
	int* array = new int[0];
	int length = 0;

	testArraySort(array, length, quickSortMethodLambda);
	testArraySort(array, length, insertSortMethodLambda);
}

TEST(SortTest, RandomArraysTest)
{
	int length = 300; // Length of generated array
	int temps = 30; // How many arrays will be tested
	for (int i = 0; i < temps; i++)
	{
		std::cout << "Test " << i + 1 << "\n";
		int* array = randomIntArray(length, 1000);
		testArraySort(array, length, quickSortMethodLambda, false);
		delete array;
		array = nullptr;
	}
}

TEST(PerformanceTest, RandomArraysPerformanceTest)
{
	int length = 100000; // Length of generated array
	int temps = 1; // How many arrays will be tested
	double averageResultTime = 0;
	for (int i = 0; i < temps; i++)
	{
		std::cout << "Test " << i + 1 << "\n";
		int* array = randomIntArray(length, 20000);
		averageResultTime += testArraySort(array, length, quickSortMethodLambda, true);
		delete array;
		array = nullptr;
	}
	std::cout << "Average result time: " << averageResultTime / temps << "ms \n";
}

/*
	Internal methods tests
*/
TEST(MedianTest, MedianOfSortedArrayTest)
{
	int array[] = { 0, 1, 3, 5, 10 };
	int length = std::size(array);

	ASSERT_EQ(median(array, array + length - 1, intComparatorLambda), 2);
}

TEST(MedianTest, MedianOfDescSortedArrayTest)
{
	int array[] = { 10, 7, 3, 2, 1 };
	int length = std::size(array);

	ASSERT_EQ(median(array, array + length - 1, intComparatorLambda), 2);
}

TEST(MedianTest, MedianOfUnSortedArrayTest)
{
	int array[] = { 10, 1, 4, 5, 3 };
	int length = std::size(array);

	ASSERT_EQ(median(array, array + length - 1, intComparatorLambda), 2);
}

TEST(MedianTest, FirstElementIsMedianTest)
{
	int array[] = { 3, 1, 10, 5, 2 };
	int length = std::size(array);

	ASSERT_EQ(median(array, array + length - 1, intComparatorLambda), 0);
}

TEST(MedianTest, LastElementIsMedianTest)
{
	int array[] = { 10, 1, 3, 5, 6 };
	int length = std::size(array);

	ASSERT_EQ(median(array, array + length - 1, intComparatorLambda), length - 1);
}

/* Utils methods for testing */
template<typename T, typename SortMethod>
double testArraySort(T* array, int length, SortMethod sortMethod, 
	bool performanceCalculation, bool debugPrintArray)
{
#ifdef _DEBUG
	std::cout << "test: ";
#endif
	if (debugPrintArray) printArray(array, length);

	if (performanceCalculation) startCounter();
	sortMethod(array, array + length - 1);
	double resultTime = 0;
	if (performanceCalculation)
	{
		resultTime = getCounter();
		std::cout << "resultTime: " << getCounter() << "\n";
	}

#ifdef _DEBUG
	std::cout << "result: ";
#endif
	if (debugPrintArray) printArray(array, length);
#ifdef _DEBUG
	std::cout << "\n";
#endif

	assert(isSorted(array, array + (length - 1), intComparatorLambda));
	return resultTime;
}

int* randomIntArray(int length, int bound)
{
	int* array = new int[length];
	for (int i = 0; i < length; i++)
	{
		array[i] = rand() % bound;
	}
	return array;
}

/* Timer */
double PCFreq = 0.0;
__int64 CounterStart = 0;

void startCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		std::cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}

double getCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}
