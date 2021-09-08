#pragma once
#include <vector>
#include <memory>

/*

@author David Wadsworth
*/


template <typename T>
class Tre
{
public:
	struct Node
	{
		Node(T value)
			: value(value)
		{}

		T value;
		std::vector<Node*> children;
	};

	Tre()
		: head_(nullptr)
	{}

	~Tre()
	{
		clear();
	}


	Node* insert(T value, Node* node = nullptr)
	{
		auto n_node = new Node(value);

		if (node)
			node->children.push_back(n_node);
		else
		{
			if (head_)
				head_->children.push_back(n_node);
			else
				head_ = n_node;
		}

		nodes_.push_back(std::unique_ptr<Node>(n_node));
	}

	void clear()
	{
		head_ = nullptr;
		nodes_.clear();
	}

	Node* get_root()
	{
		return head_;
	}

private:
	Node* head_;
	std::vector<std::unique_ptr<Node>> nodes_;
};