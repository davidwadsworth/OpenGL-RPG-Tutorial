#pragma once
#include "component_rect.h"
#include "rect.h"
#include "action.h"

namespace Component {
	class ARect : public Rectangle, public Action 
	{};
}