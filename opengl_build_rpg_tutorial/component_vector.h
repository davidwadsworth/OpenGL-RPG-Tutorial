#pragma once
#include "component.h"
#include <vector>
#include "component_collider_aabb.h"
#include "component_system.h"

namespace Component
{
	template <typename T>
	struct Vector : public IComponent, public std::vector<T> {};
#define SystemVector Vector<Component::ISystem*>
#define GJKVector Vector<Component::Collider::IGJK*>
#define AABBVector Vector<Component::Collider::AABB*>
}