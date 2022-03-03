#pragma once
#include "component_trigger_load.h"
#include "component_rect.h"

namespace Component {
	namespace Trigger {
		namespace Load
		{
			class Box : public Component::Trigger::ILoad
			{
				Rect speech_arrow_rect_;
				bool is_speech_arrow_;
				glm::vec2 pos_;
				int box_i_;
				nlohmann::json box_json_;
			public:

				void load(nlohmann::json json)
				{
					box_i_ = 0;
				}

				void execute(Entity* gamestate) override
				{
					if (box_i_ > 1)
						Logger::error("load box called more than twice.", Logger::HIGH);

					std::string textbox_name = box_json_["textbox"]["filename"];
					std::string box_name = box_json_["textbox"]["boxname"];

					auto e_box = gamestate->get_child(textbox_name)->get_child(box_name);

					auto& c_position = *e_box->get_component<Component::Position>("position");
					if (box_i_)
					{
						c_position.x = Game::removed.x;
						c_position.y = Game::removed.y;
					}
					else
					{
						c_position.x = pos_.x;
						c_position.y = pos_.y;
					}
					
					if (!is_speech_arrow_)
					{
						auto& c_speech_arrow_trans = *e_box->get_component<Component::Transform>("speech_arrow");

						if (box_i_)
							c_speech_arrow_trans.set(speech_arrow_rect_);
						else
						{
							speech_arrow_rect_.set(c_speech_arrow_trans);
							c_speech_arrow_trans.set(Game::removed.x, Game::removed.y, 0.0f, 0.0f);
						}	
					}

					box_i_++;
				}
			};
		}
	}
}