#pragma once
#include "action.h"
#include "component_rect_gjk.h"


namespace Component {
	namespace Rectangle {
		namespace GJK
		{
			class Action : public Component::Rectangle::IGJK, public Act
			{
			public:
				using Component::Rectangle::IGJK::IGJK;
			};
		}
	}
}