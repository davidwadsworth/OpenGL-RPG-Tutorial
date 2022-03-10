#pragma once
#include "component.h"
#include "tree.h"
#include "component_trigger.h"

/*
Component implementation of the tree data structure

@author David Wadsworth
*/

namespace Component
{
	template <typename T>
	class Tree : public IComponent, public Tre<T>
	{};
#define TriggerTree Tree<std::vector<Component::ITrigger*>>
}