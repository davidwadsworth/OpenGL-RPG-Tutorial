#pragma once
#include "navigator.h"
#include "component_controller.h"
#include "component_cursor.h"
#include "json.hpp"

namespace Navigator
{
	class OptionBoxH : public INavigator
	{
		Component::IController* controller_;
		Component::Cursor* cursor_;
	public:

		void init(Entity* e_gamestate)
		{
			controller_ = Game::global->get_component<Component::IController>(json_["controller"].get<std::string>());
			Entity* e = e_gamestate;
			for (auto i = 0; i < json_["cursor"].size() - 1; ++i)
				e = e->get_child(json_["cursor"][i].get<std::string>());
			cursor_ = e->get_component<Component::Cursor>((json_["cursor"].end() - 1)->get<std::string>());
		}

		int navigate() override
		{
			if (controller_->key_press_action_1())
				return cursor_->get_cursor_pos() + 1;

			if (controller_->key_press_action_2())
				return cursor_->get_cursor_size() + 2;

			if (controller_->key_down_left())
				cursor_->decrement();

			if (controller_->key_down_right())
				cursor_->increment();

			return 0;
		}
	};
}