#pragma once
#include "navigator.h"
#include "component_controller.h"
#include "component_cursor.h"
#include "json.hpp"

namespace Navigator
{
	class OptionBoxH : public INavigator
	{
		Component::IController& controller_;
		Component::Cursor& cursor_;
	public:
		OptionBoxH(Entity* gamestate, nlohmann::json json)
			: controller_(*Game::global->get_component<Component::IController>("controller")),
			cursor_(*gamestate->get_child(json["textbox"].get<std::string>())->get_component<Component::Cursor>("cursor"))
		{}

		int navigate() override
		{
			if (controller_.key_press_action_1())
				return cursor_.get_cursor_pos() + 1;

			if (controller_.key_press_action_2())
				return cursor_.get_cursor_size() + 2;

			if (controller_.key_down_left())
				cursor_.decrement();

			if (controller_.key_down_right())
				cursor_.increment();

			return 0;
		}
	};
}