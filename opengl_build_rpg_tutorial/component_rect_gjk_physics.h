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
				virtual void resolve(Component::Rectangle::IGJK& col) = 0;
			};
		}
	}
}