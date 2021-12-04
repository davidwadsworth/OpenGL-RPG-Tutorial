#pragma once
#include "component.h"
#include "doubly_linked_list.h"
#include "component_system.h"

namespace Component
{
	template <typename T>
	class DLL : public IComponent, public DoublyLinkedList<T>
	{};
#define SystemList DLL<Component::ISystem*>
}

