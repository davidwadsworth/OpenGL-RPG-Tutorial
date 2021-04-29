#pragma once
#include "component.h"
#include <vector>
#include "component_collider_aabb.h"
#include "component_system.h"

namespace Component
{
	template <typename T>
	struct Vector : public Comp, public std::vector<T> {};
#define SystemVector Vector<Component::Sys*>
#define ColliderVector Vector<Component::Col*>
#define AABBVector Vector<Component::Collider::AABB*>
}