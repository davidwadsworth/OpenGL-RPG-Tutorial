#pragma once
/**
doubly linked list custom implementation

@author David Wadsworth
*/

template <typename T>
class DoublyLinkedList
{
public:
	DoublyLinkedList()
		: head_(nullptr)
	{}
	~DoublyLinkedList()
	{
		while (head_)
			remove(&head_);
	}

	struct Node
	{
		T value;
		Node* next,
			* previous;
	};

	void remove(Node** node)
	{
		if (!node || !*node)
			return;

		if ((*node)->next)
			(*node)->next->previous = (*node)->previous;
		if ((*node)->previous)
			(*node)->previous->next = (*node)->next;

		if (*node == head_)
			head_ = head_->next;

		delete* node;

		*node = nullptr;
	}

	Node* push_back(T item)
	{
		auto old_head = head_;
		head_ = new Node{ item, old_head, nullptr };

		if (old_head)
			old_head->previous = head_;

		return head_;
	}

	Node* insert(T item, GLuint pos)
	{
		Node* head_iter = head_;
		for (auto i = 1; i < pos && head_iter; ++i)
		{
			if (head_iter->next)
				head_iter = head_iter->next;
			else
				break;
		}
		auto insert = new Node{ item, head_iter, nullptr };
		if (head_iter)
		{
			insert->previous = head_iter->previous;
			head_iter->previous = insert;
		}

		return insert;
	}

	Node* get_begin()
	{
		return head_;
	}

private:
	Node* head_;
};