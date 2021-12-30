#pragma once
#include "component.h"
#include "quadly_linked_tree.h"
#include "component_trigger.h"

namespace Component
{
	template <typename T>
	class QLTree : public IComponent, public QuadlyLinkedTree<T>
	{};
}
#define QLTriggerTree QLTree<Component::ITrigger*>