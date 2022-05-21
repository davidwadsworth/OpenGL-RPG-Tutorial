#pragma once
#include <vector>
#include <memory>

/*
It says its a tree and eventually it will be a tree, but for now its just a linked list :|
useful for traversing events in the video game.

@author David Wadsworth
*/

template <typename T>
class Tree
{
public:
	std::vector<Tree*> children_;

	Tree() = default;
	~Tree() = default;

	T val;
};