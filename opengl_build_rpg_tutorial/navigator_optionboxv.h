#pragma once
#include "component_controller.h"
#include "component_cursor.h"
#include "navigator.h"
#include "json.hpp"


namespace Navigator
{
	class OptionBoxV : public INavigator
	{
		Component::IController& controller_;
		Component::Cursor& cursor_;
	public:
		OptionBoxV(Entity* gamestate, nlohmann::json json)
			: controller_(*Game::global->get_component<Component::IController>("controller")),
			cursor_(*gamestate->get_child(json["textbox"].get<std::string>())->get_component<Component::Cursor>("cursor"))
		{}

		NPATH navigate() override
		{
			if (controller_.key_press_action_1())
				return NPATH(cursor_.get_cursor_pos() + 1);

			if (controller_.key_down_down())
				cursor_.decrement();

			if (controller_.key_down_up())
				cursor_.increment();

			return NPATH::STAY;
		}
	};
}