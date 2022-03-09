#pragma once
#include "component.h"
#include "tree.h"
#include "component_trigger.h"

/*
TODO

@author David Wadsworth
*/

namespace Component
{
	template <typename T>
	class Tree : public IComponent, public Tre<T>
	{};
#define TriggerTree Tree<std::vector<Component::ITrigger*>>
}