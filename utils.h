#pragma once

template<typename T>
void printArray(const T* first, const int lenght);

template<typename T, typename Compare>
bool isSorted(const T* first, const T* last, Compare comp);

#include "utils.hpp"
