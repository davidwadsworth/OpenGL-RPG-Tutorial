#pragma once
#include "action.h"
#include "component_rect_gjk.h"

/*
Component gjk rect with action attached

@author David Wadsworth
*/

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