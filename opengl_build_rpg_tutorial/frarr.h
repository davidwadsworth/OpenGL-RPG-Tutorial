#pragma once
#include <glad/glad.h>
#include "logger.h"
#include <vector>
/*
 * FRArr : fast resizable array
 * addition adds the element to end of list unless its full in which case it does nothing
 * remove displaces the last element with the position of remove
 * clear brings the array back to its starting point
 */

template <typename T>
struct FRArr
{
private:
	std::vector<T> buffer_;
public:
	unsigned size, max_size;

	FRArr() // creates unusable, unmaloced unruly
		: size(0), max_size(0)
	{}

	FRArr(GLuint max_size) // creates a dynamically sized array
		: max_size(max_size), size(0)
	{
		buffer_.resize(max_size);
	}

	~FRArr()
	{
		buffer_.clear();
	}

	void push_back(T col) // places T to the end of the unruly and increments size, if full then nothing is added
	{
		if (size >= max_size) 
			Logger::error("FRArr is full. Data might be lost", Logger::HIGH);
		buffer_[size++] = col;
	}

	void remove(unsigned ele) // replaces the element with the element at the end
	{
		if (ele >= size)
			Logger::error("Deleting element at pos " + ele + " cannot be accomplished" , Logger::HIGH);
		buffer_[ele] = buffer_[--size];
	}

	GLboolean is_full()
	{
		return size == max_size - 1;
	}

	void clear() {
		size = 0;
	}

	T& get(unsigned i)
	{
		if (i >= size)
			Logger::error("cannot get element at pos " + i, Logger::HIGH);
		return buffer_[i];
	}

	std::vector<T>& iterator()
	{
		return buffer_;
	}

	T& operator[](unsigned i)
	{
		return buffer_[i];
	}

};