#pragma once
#include "component.h"
#include "doubly_linked_list.h"
#include "component_system.h"
#include <vector>

namespace Component
{
	template <typename T>
	struct Observer : public IComponent
	{
		std::vector<DoublyLinkedList<T*>::Node*> observed_objects;
	};

#define SystemObserver Observer<Component::ISystem>
}