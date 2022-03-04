#pragma once
#include "component_rect.h"
#include "rect.h"
#include "action.h"

namespace Component {
	class ARect : public Component::Rectangle, public Action 
	{
		using Component::Rectangle::Rectangle;
	};
}