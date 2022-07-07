#pragma once
#include "component_trigger_load.h"
#include "component_rect.h"
#include "game.h"

namespace Component {
	namespace Trigger {
		namespace Load
		{
			class SwitchState : public Component::Trigger::ILoad
			{
				glm::vec2 return_pos_;
				std::string next_state_;
			public:

				void load(nlohmann::json json)
				{
					float x = json["return_pos"]["x"];
					float y = json["return_pos"]["y"];
					return_pos_ = glm::vec2(x, y);
					next_state_ = json["next_state"];
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
	}
}