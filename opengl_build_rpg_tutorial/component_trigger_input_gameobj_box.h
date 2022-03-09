#pragma once
#include "component_trigger_input.h"
#include "component_rect.h"
#include "rect.h"
#include <glm/vec2.hpp>
#include "component_material.h"
#include "component_system_render_draw.h"
#include "component_trigger_input.h"
#include "component_engine.h"
#include "component_system_render_offset.h"
#include "delimiter_split.h"
#include "component_json.h"

/*
Creates our local camera using width and height

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace GameObj
			{
				class Box : public Component::Trigger::IInput
				{
					void create(Entity* gamestate) override final
					{
						auto box_info = delimiter_split(name_.c_str(), '_');

						nlohmann::json box_json = gamestate->get_child("index")->get_component<Component::Json>(box_info[0])->json;

						std::string spritesheet_name = box_json["texture"];

						auto& c_box_position = *entity_->add_id_component<Component::Position>("position", Game::removed);
						
						auto box_x = 0.0f;
						auto box_y = 0.0f;
						float box_w = box_json["width"];
						float box_h = box_json["height"];

						float box_scale = box_json["scale"];
						
						box_w *= box_scale;
						box_h *= box_scale;

						float corner_size = box_json["corner_size"];
						float render_group = box_json["render_group"];

						auto scaled_corner_size = corner_size * box_scale;

						auto& c_renderer = *gamestate->get_component<Component::Renderer>("renderer");

						std::vector<Component::Transform*> transform_vec;
						// box corners transforms
						transform_vec.push_back(entity_->add_id_component<Component::Transform>("top_right",
							Rect( box_x, box_y, scaled_corner_size, scaled_corner_size )));
						transform_vec.push_back(entity_->add_id_component<Component::Transform>("top_left",
							Rect( box_x + box_w - scaled_corner_size, box_y, scaled_corner_size, scaled_corner_size )));
						transform_vec.push_back(entity_->add_id_component<Component::Transform>("bot_right",
							Rect( box_x, box_y + box_h - scaled_corner_size, scaled_corner_size, scaled_corner_size )));
						transform_vec.push_back(entity_->add_id_component<Component::Transform>("bot_left",
							Rect( box_x + box_w - scaled_corner_size, box_y + box_h - scaled_corner_size, scaled_corner_size, scaled_corner_size )));

						// box side transforms
						transform_vec.push_back(entity_->add_id_component<Component::Transform>("top_side",
							Rect( box_x + scaled_corner_size, box_y, box_w - scaled_corner_size * 2, scaled_corner_size )));
						transform_vec.push_back(entity_->add_id_component<Component::Transform>("left_side",
							Rect( box_x, box_y + scaled_corner_size, scaled_corner_size, box_h - scaled_corner_size * 2.0f )));
						transform_vec.push_back(entity_->add_id_component<Component::Transform>("right_side",
							Rect(box_x + box_w - scaled_corner_size, box_y + scaled_corner_size, scaled_corner_size, box_h - scaled_corner_size * 2.0f)));
						transform_vec.push_back(entity_->add_id_component<Component::Transform>("bot_side",
							Rect( box_x + scaled_corner_size, box_y + box_h - scaled_corner_size, box_w - scaled_corner_size * 2.0f, scaled_corner_size )));

						// center transform
						transform_vec.push_back(entity_->add_id_component<Component::Transform>("center",
							Rect( box_x + scaled_corner_size, box_y + scaled_corner_size, box_w - scaled_corner_size * 2.0f, box_h - scaled_corner_size * 2.0f )));

						// speech arrow
						transform_vec.push_back(entity_->add_id_component<Component::Transform>("speech_arrow", box_x + scaled_corner_size, box_y + box_h - scaled_corner_size, scaled_corner_size * 2.0f, scaled_corner_size));

						// get textbox shader and texture
						auto e_spritesheet = gamestate->get_child(spritesheet_name);
						auto& c_ss_material = *e_spritesheet->get_component<Component::Material>("material");


						// get src rects
						std::vector<Component::Src*> box_srcs;
						for (std::string texture_id : box_json["texture_ids"])
							box_srcs.push_back(e_spritesheet->get_component<Component::Src>(texture_id));

						auto cs_item = entity_->add_id_component<Component::System::Render::Offset>("render", c_box_position, box_srcs, transform_vec, c_ss_material, c_renderer);

						auto& render_engine = *gamestate->get_component<Component::Engine>("render");
						render_engine.add(cs_item, render_group);
					}
				public:
					using Component::Trigger::IInput::IInput;

				};
			}
		}
	}
}