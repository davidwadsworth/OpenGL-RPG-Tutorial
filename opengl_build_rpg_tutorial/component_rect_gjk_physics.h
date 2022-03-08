#pragma once
#include "component_rect_gjk_action.h"

namespace Component {
	namespace Rectangle {
		namespace GJK
		{
			template <typename T>
			class IPhysics : public T
			{
			public:
				using T::T;
				virtual void resolve(Component::Rectangle::IGJK& col) = 0;
			};
#define PhysicsAction IPhysics<Component::Rectangle::GJK::Action>
#define PhysicsNorm IPhysics<Component::Rectangle::IGJK>
		}
	}
}