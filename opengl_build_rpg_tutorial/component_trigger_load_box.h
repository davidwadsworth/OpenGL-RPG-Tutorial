#pragma once
#include "component_trigger_load.h"
#include "component_transform.h"

namespace Component {
	namespace Trigger {
		namespace Load
		{
			class Box : public Component::Trigger::ILoad
			{
			public:

				void execute(Entity* gamestate) override
				{
					std::string textbox_name = json_["textbox"][0];
					int textbox_pos = json_["textbox"][1];

					auto e_box = gamestate->get_child(textbox_name)->get_child(textbox_pos)->get_child("box");

					float box_x = json_["box_rect"][0];
					float box_y = json_["box_rect"][1];
					float box_w = json_["box_rect"][2];
					float box_h = json_["box_rect"][3];
					float box_sc = json_["box_scale"];
					int corner_size = json_["corner_size"];
					bool speech_arrow = json_["speech_arrow"] != "none";
					
					auto scaled_corner_size = corner_size * box_sc;


					// box corners transforms
					auto& c_tl_corner_trans = *e_box->get_component<Component::Transform>(0);
					c_tl_corner_trans.set(box_x, box_y, scaled_corner_size, scaled_corner_size);
					auto& c_tr_corner_trans = *e_box->get_component<Component::Transform>(1);
					c_tr_corner_trans.set(box_x + box_w - scaled_corner_size, box_y, scaled_corner_size, scaled_corner_size);
					auto& c_bl_corner_trans = *e_box->get_component<Component::Transform>(2);
					c_bl_corner_trans.set(box_x, box_y + box_h - scaled_corner_size, scaled_corner_size, scaled_corner_size);
					auto& c_br_corner_trans = *e_box->get_component<Component::Transform>(3);
					c_br_corner_trans.set(box_x + box_w - scaled_corner_size, box_y + box_h - scaled_corner_size, scaled_corner_size, scaled_corner_size);

					// box side transforms
					auto& c_t_side_trans = *e_box->get_component<Component::Transform>(4);
					c_t_side_trans.set(box_x + scaled_corner_size, box_y, box_w - scaled_corner_size * 2, scaled_corner_size);
					auto& c_l_side_trans = *e_box->get_component<Component::Transform>(5);
					c_l_side_trans.set(box_x, box_y + scaled_corner_size, scaled_corner_size, box_h - scaled_corner_size * 2);
					auto& c_b_side_trans = *e_box->get_component<Component::Transform>(6);
					c_b_side_trans.set(box_x + scaled_corner_size, box_y + box_h - scaled_corner_size, box_w - scaled_corner_size * 2, scaled_corner_size);
					auto& c_r_side_trans = *e_box->get_component<Component::Transform>(7);
					c_r_side_trans.set(box_x + box_w - scaled_corner_size, box_y + scaled_corner_size, scaled_corner_size, box_h - scaled_corner_size * 2);

					// center transform
					auto& c_center_trans = *e_box->get_component<Component::Transform>(8);
					c_center_trans.set(box_x + scaled_corner_size, box_y + scaled_corner_size, box_w - scaled_corner_size * 2, box_h - scaled_corner_size * 2);

					if (speech_arrow)
					{
						auto& c_speech_arrow_trans = *e_box->get_component<Component::Transform>(9);
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