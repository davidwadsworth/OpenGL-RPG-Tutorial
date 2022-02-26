#pragma once
#include "component_transform.h"
#include "entity.h"
#include "component_collider_gjk.h"
#include "component_collider_aabb.h"
#include "component_system.h"
#include "component_trigger.h"

namespace Component
{
	template <typename T> struct QuadTree;

	template <typename T>
	struct IQuadTreeKey : public IComponent
	{
	protected:
		Component::Transform& transform_;
		T& output_;
		bool is_retrieved_;
	public:
		friend class QuadTree<T>;
		QuadTreeKey(T& output, Component::Transform& transform)
			: output_(output), transform_(transform), is_retrieved(false)
		{}
	};

#define GJKQuadTreeKey QuadTreeKey<Component::Collider::IGJK>
#define AABBQuadTreeKey QuadTreeKey<Component::Collider::AABB>
#define SystemQuadTreeKey QuadTreeKey<Component::ISystem>
#define TriggerQuadTreeKey QuadTreeKey<Component::ITrigger>
}