#pragma once

// Constructors
template<typename T>
Array<T>::Array(const int capacity) : _capacity(capacity), _size(0)
{
	_array = new T[_capacity];
}

template<typename T>
Array<T>::Array(const Array& a) : _capacity(a._capacity), _size(a._size)
{
	//std::cout << "Copy constructor" << "\n";
	_array = new T[_capacity];
	for (int i = 0; i < _size; i++)
	{
		_array[i] = a._array[i];
	}
}

template <typename T>
Array<T>::Array(Array&& a) noexcept : _capacity(a._capacity), _size(a._size)
{
	//std::cout << "Move constructor" << "\n";
	_array = a._array;
	a._array = nullptr;
}

// Destructor
template<typename T>
Array<T>::~Array()
{
	delete[] _array;
}

// Override operators
template<typename T>
Array<T>& Array<T>::operator=(const Array& a)
{
	//std::cout << "Copy assignment" << "\n";
	
	// Check self-assignment
	if (&a == this)
		return *this;

	delete[] _array;
	_array = a._array;
	a._array = nullptr;
	_capacity = a._capacity;
	_size = a._size;

	return *this;
}

template <typename T>
Array<T>& Array<T>::operator=(Array&& a) noexcept
{
	//std::cout << "Move assignment" << "\n";
	
	// Check self-assignment
	if (&a == this)
		return *this;
	
	delete[] _array;
	_array = a._array;
	a._array = nullptr;
	_capacity = a._capacity;
	_size = a._size;

	return *this;
}

template<typename T>
T& Array<T>::operator[](int index)
{
	assert(index >= 0 && index < _size);
	
	return _array[index];
}

template<typename T>
const T& Array<T>::operator[](int index) const
{
	assert(index >= 0 && index < _size);

	return _array[index];
}

// Content manipulating functions
template<typename T>
int Array<T>::size() const
{
	return _size;
}

template<typename T>
int Array<T>::capacity() const
{
	return _capacity;
}

template<typename T>
void Array<T>::insert(const T& value)
{
	if (_size == _capacity)
	{
		resize(_capacity * CAPACITY_MULTIPLIER);
	}

	_array[_size] = value;
	_size++;
}

template<typename T>
void Array<T>::insert(int index, const T& value)
{
	assert(index >= 0 && index <= _size);

	if (_size == _capacity)
	{
		resize(_capacity * CAPACITY_MULTIPLIER);
	}

	for (int i = _size; i > index; i--)
	{
		_array[i] = std::move(_array[i - 1]);
	}

	_array[index] = value;
	_size++;
}

template<typename T>
void Array<T>::remove(int index)
{
	assert(index >= 0 && index < _size);

	_size--;
	for (int i = index; i < _size; i++)
	{
		_array[i] = std::move(_array[i + 1]);
	}
}

// Iterator implementation
template<typename T>
Array<T>::Iterator::Iterator(Array<T>& array) : _index(0), _target_array(array)
{
}

template<typename T>
const T& Array<T>::Iterator::get() const
{
	return _target_array[_index];
}

template<typename T>
void Array<T>::Iterator::set(const T& value)
{
	_target_array[_index] = value;
}

template<typename T>
void Array<T>::Iterator::insert(const T& value)
{
	_target_array.insert(_index, value);
}

template<typename T>
void Array<T>::Iterator::remove()
{
	_target_array.remove(_index);
}

template<typename T>
void Array<T>::Iterator::next()
{
	_index++;
}

template<typename T>
void Array<T>::Iterator::prev()
{
	_index--;
}

template<typename T>
void Array<T>::Iterator::to_index(int index)
{
	this->_index = index;
}

template<typename T>
bool Array<T>::Iterator::has_next() const
{
	return _index < _target_array.size() - 1;
}

template<typename T>
bool Array<T>::Iterator::has_prev() const
{
	return _index > 0;
}

template<typename T>
typename Array<T>::Iterator Array<T>::iterator()
{
	Iterator iterator(*this);

	return iterator;
}

template<typename T>
const typename Array<T>::Iterator Array<T>::iterator() const
{
	Iterator iterator(*this);

	return iterator;
}

template<typename T>
void Array<T>::resize(int new_capacity)
{
	//std::cout << "Resize" << "\n";

	Array<T> array_copy(new_capacity);

	array_copy._size = _size;
	for (int i = 0; i < _size; i++)
	{
		array_copy[i] = std::move(_array[i]);
	}

	*this = std::move(array_copy);
}