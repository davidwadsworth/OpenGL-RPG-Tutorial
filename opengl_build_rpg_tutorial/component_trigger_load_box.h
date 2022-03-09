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
				int box_i_ = 2;
				std::string textbox_name_, box_name_;
			public:

				void load(nlohmann::json json)
				{
					float item_x = json["rect"]["x"];
					float item_y = json["rect"]["y"];
					float item_w = json["rect"]["w"];
					float offset_y = json["box"]["offset"];
					
					float box_h = json["box"]["height"];

					float box_corner_size = json["box"]["corner_size"];
					float box_scale = json["box"]["scale"];

					auto scaled_corner = box_corner_size * box_scale;

					auto offset_x = item_w / 2.0f - scaled_corner * 2.0f;
					pos_.x = item_x + offset_x;
					pos_.y = item_y + offset_y - box_h;
					is_speech_arrow_ = json["message"]["speech_box"] == "true";
					textbox_name_ = json["textbox"];
					box_i_ = 0;
				}

				void execute(Entity* gamestate) override
				{
					if (box_i_ > 1)
						Logger::error("load box called more than twice or load not called.", Logger::HIGH);

					auto e_box = gamestate->get_child(textbox_name_)->get_child("box");

					auto& c_position = *e_box->get_component<Component::Position>("position");
					if (box_i_)
					{
						c_position.x = Game::removed.x;
						c_position.y = Game::removed.y;
					}
					else
					{
						c_position.x = -pos_.x;
						c_position.y = -pos_.y;
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