#pragma once
#include <vector>
#include <memory>

template <typename T>
class Tre
{
public:
	struct Node
	{
		Node* next;
		T item;
	};

	Tre()
		: head_(nullptr)
	{}

	~Tre()
	{
		destroy();
	}

	void add(T item)
	{
		auto temp_head = head_;
		head_ = new Node{ temp_head, item };
	}

	void destroy()
	{
		nodes_.clear();
		head_ = nullptr;
	}

	Node* get_head()
	{
		return head_;
	}
private:
	std::vector<std::unique_ptr<Node>> nodes_;
	Node* start_;
};