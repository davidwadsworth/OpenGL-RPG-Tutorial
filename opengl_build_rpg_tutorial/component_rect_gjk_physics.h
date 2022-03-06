#pragma once
#include "component_rect_gjk.h"

namespace Component {
	namespace Rect {
		namespace GJK
		{
			template <typename T>
			class IPhys : public T
			{
			public:
				virtual void resolve(T& col) = 0;
			};
#define PhysicsAction IPhys<GJKAction>
#define IPhysics IPhys<IGJK>
		}
	}
}