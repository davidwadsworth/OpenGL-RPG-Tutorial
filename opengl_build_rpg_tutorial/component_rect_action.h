#pragma once
#include "component_rect.h"
#include "rect.h"
#include "action.h"

namespace Component {
	namespace Rect 
	{
		class Action : public Component::Rectangle
		{
		public:
			nlohmann::json action;
			using Component::Rectangle::Rectangle;
		};
	}
}