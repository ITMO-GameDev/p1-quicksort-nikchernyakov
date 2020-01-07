#pragma once

template <typename T>
class Array final
{
public:

	// Constructors
	explicit Array(const int capacity = 8);
	Array(const Array& a);
	Array(Array&& a) noexcept;

	// Destructor
	~Array();

	// Override operators
	Array& operator=(const Array &a);
	Array& operator=(Array&& a) noexcept;
	T& operator[](int index);
	const T& operator[](int index) const;

	// Content manipulating functions
	int size() const;
	int capacity() const;
	void insert(const T& value);
	void insert(int index, const T& value);
	void remove(int index);

	// Iterator declaration
	class Iterator
	{
	public:

		explicit Iterator(Array<T>& array);

		const T& get() const;
		void set(const T& value);
		void insert(const T& value);
		void remove();
		void next();
		void prev();
		void to_index(int index);
		bool has_next() const;
		bool has_prev() const;

	private:
		int _index;
		Array<T>& _target_array;
	};

	Iterator iterator();
	const Iterator iterator() const;

private:

	const static int CAPACITY_MULTIPLIER = 2;

	T* _array;
	int _capacity;
	int _size;

	void resize(int new_capacity);
};


#include "array.hpp";
