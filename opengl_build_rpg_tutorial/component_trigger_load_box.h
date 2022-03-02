#pragma once
#include "component_trigger_load.h"
#include "component_rect.h"

namespace Component {
	namespace Trigger {
		namespace Load
		{
			class Box : public Component::Trigger::ILoad
			{
			public:
				void execute(Entity* gamestate) override
				{
					std::string textbox_name = json_["textbox"]["filename"];
					int textbox_pos = json_["textbox"]["pos"];

					auto e_box = gamestate->get_child(textbox_name)->get_child(textbox_pos)->get_child("box");

					float box_x = json_["box_rect"]["x"];
					float box_y = json_["box_rect"]["y"];
					float box_w = json_["box_rect"]["w"];
					float box_h = json_["box_rect"]["h"];
					float box_sc = json_["box_scale"];
					int corner_size = json_["corner_size"];
					bool speech_arrow = json_["speech_arrow"] != "none";
					
					auto scaled_corner_size = corner_size * box_sc;

					auto& c_position = *e_box->get_component<Component::Position>("position");
					c_position.x = box_x;
					c_position.y = box_y;

					if (speech_arrow)
					{
						auto& c_speech_arrow_trans = *e_box->get_component<Component::Transform>("speech_arrow");
						if (json_["speech_arrow"] == "left")
							c_speech_arrow_trans.set(box_x + scaled_corner_size, box_y + box_h - scaled_corner_size, scaled_corner_size * 2.0f, scaled_corner_size);
						else 
							c_speech_arrow_trans.set(box_x + box_w - scaled_corner_size * 3.0f, box_y + box_h - scaled_corner_size, scaled_corner_size * 2.0f, scaled_corner_size);
					}

				}
			};
		}
	}
}