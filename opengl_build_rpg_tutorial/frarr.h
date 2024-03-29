#pragma once
#include <glad/glad.h>
#include "logger.h"
#include <vector>
#include <string>
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

	void push_back(T col) // places T to the end of the frarr and increments size, if full then nothing is added
	{
		if (size >= max_size) 
			Logger::error("FRArr is full. Data might be lost", Logger::HIGH);
		buffer_[size++] = col;
	}

	void push_back(std::vector<T> cols) // places a vector of T to the end of frarr
	{
		if (size + cols.size() - 1 >= max_size)
			Logger::error("FRArr is full. Data might be lost", Logger::HIGH);

		buffer_.insert(buffer_.end(), cols.begin(), cols.end());
	}

	void push_back(FRArr<T>& frarr)
	{
		push_back(frarr.buffer_);
	}

	void remove(std::size_t ele) // replaces the element with the element at the end
	{
		if (ele >= size)
			Logger::error("Deleting element at pos " + std::to_string(ele) + " cannot be accomplished" , Logger::HIGH);
		buffer_[ele] = buffer_[--size];
	}

	GLboolean is_full()
	{
		return size == max_size - 1;
	}

	void clear() {
		size = 0;
	}

	T& get(std::size_t i)
	{
		if (i >= size)
			Logger::error("cannot get element at pos " + std::to_string(i), Logger::HIGH);
		return buffer_[i];
	}

	std::vector<T>& iterator()
	{
		return buffer_;
	}

	T& operator[](std::size_t i)
	{
		return buffer_[i];
	}

};