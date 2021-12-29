#pragma once
#include <vector>
#include <memory>

/*

@author David Wadsworth
*/


template <typename T>
class QuadlyLinkedTree
{
public:
	QuadlyLinkedTree()
		: head_(nullptr)
	{}
	~QuadlyLinkedTree()
	{
		clear();
	}

	void clear()
	{
		head_ = nullptr;
		nodes_.clear();
	}

	enum Direction
	{
		direction_up,
		direction_down,
		diection_right,
		direction_left
	};

	struct Node
	{
		T value;
		Node* up,
			* down,
			* right,
			* left;
	};

	Node* push_right(T item)
	{
		return head_ = insert(item, head_, Direction::direction_left);
	}

	Node* push_up(T item)
	{
		return head_ = insert(item, head_, Direction::direction_down);
	}

	Node* push_left(T item)
	{
		return head_ = insert(item, head_, Direction::direction_right);
	}

	Node* push_down(T item)
	{
		return head_ = insert(item, head_, Direction::direction_up);
	}

	Node* get_begin()
	{
		return head_;
	}

private:
	Node* insert(T item, Node* node, Direction direction)
	{
		if (!node)
		{
			clear();
			head_ = new Node{ item, nullptr, nullptr, nullptr, nullptr };
			head_->up = head_->down = head_->left = head_->right = head_;
			return head_;
		}

		Node* ins = nullptr;
		switch (direction)
		{
		case Direction::direction_up:
			ins = new Node{ item, node->up, node, nullptr, nullptr };
			ins->right = ins->left = ins;
			if (node->up)
				node->up->down = ins;
			node->up = ins;
			break;
		case Direction::direction_down:
			ins = new Node{ item, node, node->down, nullptr, nullptr };
			ins->right = ins->left = ins;
			if (node->down)
				node->down->up = ins;
			node->down = ins;
			break;
		case Direction::diection_right:
			ins = new Node{ item, nullptr, nullptr, node->right, node };
			ins->down = ins->up = ins;
			if (node->right)
				node->right->left = ins;
			node->right = ins;
			break;
		case Direction::direction_left:
			ins = new Node{ item, nullptr, nullptr, node, node->left };
			ins->down = ins->up = ins;
			if (node->left)
				node->left->right = ins;
			node->left = ins;
			break;
		default:
			throw;
			break;
		}
		nodes_.push_back(std::unique_ptr<Node>(ins));
		return ins;
	}

	std::vector<std::unique_ptr<Node>> nodes_;
	Node* head_;
};