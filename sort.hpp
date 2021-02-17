#pragma once

#include <utility>
#include "utils.h"

const int SORT_THRESHOLD = 25;

template<typename T, typename Compare>
void sort(T* first, T* last, Compare comp)
{
	int leftInd = 0;
	int rightInd = last - first;
	while (leftInd < rightInd)
	{
#ifdef _DEBUG
		printArray(first, (last - first) + 1);
		std::cout << "Pair: " << leftInd << " " << rightInd << "\n";
#endif

		T* left = first + leftInd;
		T* right = first + rightInd;
		int length = (right - left) + 1;
		
		if (length <= SORT_THRESHOLD)
		{
			// Use insertation sort for small parts
			insertSort(left, right, comp);
			return;
		}

		// Median optimization
		int med = median(left, right, comp);
		std::swap(left[med], left[(length - 1) / 2]);
#ifdef _DEBUG
		std::cout << "med: " << med << " " << (length - 1) / 2 << "\n";
		printArray(first, (last - first) + 1);
#endif

		int j = partition(left, right, comp); // Find pivot

		// Handle biggest part recursively and less part iteratively
		if (j > rightInd - (leftInd + j))
		{
			// Left part is biggest
			sort(left, left + j, comp);
			leftInd += j + 1;
		}
		else
		{
			// Right part is biggest
			sort(left + j + 1, right, comp);
			rightInd = leftInd + j;
		}
	}
}

template<typename T, typename Compare>
void insertSort(T* first, T* last, Compare comp)
{
	int length = (last - first) + 1;
	for (int currentInd = 1; currentInd < length; currentInd++)
	{
		int checkInd = currentInd;
		while (checkInd > 0 && comp(first[checkInd - 1], first[checkInd]))
		{
			std::swap(first[checkInd - 1], first[checkInd]);
			checkInd--;
		}
	}
}

template<typename T, typename Compare>
int partition(T* first, T* last, Compare comp)
{
	T v = first[(last - first) / 2];
#ifdef _DEBUG
	std::cout << "v: " << v << "\n";
#endif
	int i = 0;
	int j = last - first;
	int lastInd = last - first;
	while (i <= j)
	{
		// Move left bound until left element would be less than pivot
		while (comp(v, first[i]))
			i++;
		// Move right bound until pivot would be less than right element
		while (comp(first[j], v))
			j--;
		if (i >= j) 
			break;
#ifdef _DEBUG
		std::cout << "swap: " << i << ":" << first[i] << " " << j << ":" << first[j] << "\n";
#endif
		std::swap(first[i++], first[j--]);

#ifdef _DEBUG
		printArray(first, (last - first) + 1);
#endif
	}

#ifdef _DEBUG
	std::cout << "part: " << i << " " << j << "\n";
#endif
	return j;
}

template<typename T, typename Compare>
int median(const T* first, const T* last, Compare comp)
{
	int length = last - first;
	int med = length / 2;
	return (comp(first[0], last[0])
		? (comp(last[0], first[med]) ? length : comp(first[0], first[med]) ? med : 0)
		: (comp(first[med], last[0]) ? length : comp(first[med], first[0]) ? med : 0));
}
