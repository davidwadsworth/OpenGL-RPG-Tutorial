#pragma once
#include "component_trigger_load.h"


namespace Component {
	namespace Trigger {
		namespace Load
		{
			class OptionBox : public Component::Trigger::ILoad
			{

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