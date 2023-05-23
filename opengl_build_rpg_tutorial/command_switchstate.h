#pragma once
#include "command.h"
#include "component_rect.h"
#include "component_json.h"
#include "game.h"

namespace ICommand
{
	class SwitchState : public ICommand
	{
		glm::vec2 return_pos_;
		std::string next_state_;
	public:

		void load(nlohmann::json json)
		{
			float x = json["load"]["data"]["return_pos"]["x"];
			float y = json["load"]["data"]["return_pos"]["y"];
			return_pos_ = glm::vec2(x, y);
			next_state_ = json["load"]["data"]["next_state"];
		}

		void execute(Entity* gamestate) override
		{
			auto& player_json = Game::global->get_child("index")->get_component<Component::Json>("player")->json;
			player_json["info"]["rect"]["x"] = return_pos_.x;
			player_json["info"]["rect"]["y"] = return_pos_.y;
			Game::set_next_state(next_state_);
		}
	};
}