#pragma once


template <typename T>
class RingBuffer
{
	std::vector<std::unique_ptr<T>> buffer_;
	std::size_t head_, tail_;
public:

	RingBuffer(std::size_t bufferSize)
		: head_(0u), tail_(0u)
	{
		buffer_.reserve(buffersize);
		std::generate_n(std::back_inserter(buffer_), bufferSize, [] { return std::make_unique<T>(); });
	}


	T* get()
	{
		auto t = buffer_[tail_++].get();
		t->clear();
		if (tail_ > buffer_.size() - 1)
			tail_ = 0u;
		if (tail_ == head_)
			Logger::error("Ring Buffer full on get()", Logger::HIGH);
		return t;
	}


	void remove(std::size_t size)
	{
		head_ += size;
		if (head_ > buffer_.size() - 1)
			head_ = buffer_.size() - 1 - head_;
	}

};