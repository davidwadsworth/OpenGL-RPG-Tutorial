#pragma once
#include "component.h"
#include "component_system.h"
#include "component_doubly_linked_list.h"
#include <vector>

namespace Component
{
	template <typename T>
	struct Observer : public IComponent
	{
		std::vector<DoublyLinkedList<ISystem*>::Node*> observed_objects;
	};

#define SystemObserver Observer<Component::ISystem>
}