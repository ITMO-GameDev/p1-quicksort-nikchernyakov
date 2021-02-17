#pragma once

template<typename T>
bool comp(const T& a, const T& b);

template<typename T, typename Compare>
void sortMethod(T* first, T* last, Compare comp);

template<typename T, typename Compare>
void sort(T* first, T* last, Compare comp);

template<typename T, typename Compare>
void insertSort(T* first, T* last, Compare comp);

template<typename T, typename Compare>
int partition(T* first, T* last, Compare comp);

template<typename T, typename Compare>
int median(const T* first, const T* last, Compare comp);

template<typename T>
void swap(T* a, T* b);

#include "sort.hpp";
