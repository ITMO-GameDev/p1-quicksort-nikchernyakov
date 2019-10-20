#pragma once

#include <iostream>

template<typename T>
void printArray(T* first, int lenght)
{
	for (int i = 0; i < lenght; i++)
	{
		std::cout << first[i] << " ";
	}
	std::cout << "\n";
}

template<typename T, typename Compare>
bool isSorted(T* first, T* last, Compare comp)
{
	int length = (last - first) + 1;
	for (int i = 0; i < length - 1; i++)
	{
		if (comp(first[i], first[i + 1]))
		{
			return false;
		}
	}
	return true;
}
