#pragma once
#include "component.h"
#include "tree.h"
#include "component_trigger.h"

namespace Component
{
	template <typename T>
	class Tree : public IComponent, public Tre<T>
	{};
#define TriggerTree Tree<Component::ITrigger>
}