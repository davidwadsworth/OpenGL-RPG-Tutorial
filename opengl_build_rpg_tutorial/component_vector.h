#pragma once
#include <vector>
#include "component_system.h"
#include "component_trigger.h"

/*
TODO

@author David Wadsworth
*/

namespace Component
{
	template <typename T>
	struct Vector : public IComponent, public std::vector<T> {};
#define SystemVector Vector<Component::ISystem*>
#define TriggerVector Vector<Component::ITrigger*>
}