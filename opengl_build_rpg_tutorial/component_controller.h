#pragma once
#include "component.h"

namespace Component
{
	struct IController : public IComponent
	{
		virtual bool key_down_left() = 0;
		virtual bool key_down_right() = 0;
		virtual bool key_down_down() = 0;
		virtual bool key_down_up() = 0;

		virtual bool key_press_action_1() = 0;
	};
}
