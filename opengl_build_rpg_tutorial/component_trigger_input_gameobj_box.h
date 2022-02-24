#pragma once
#include "component_trigger_input.h"
#include "component_transform.h"
#include "rect.h"
#include <glm/vec2.hpp>
#include "component_src.h"
#include "component_material.h"
#include "component_system_render_draw.h"
#include "component_trigger_input_gameobj.h"
#include "component_system_item.h"
#include "component_engine.h"

/*
Creates our local camera using width and height

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace GameObj
			{
				class Box : public Component::Trigger::Input::IGameObj
				{
				private:
					void init(Entity* gamestate) override final
					{
						auto box_info = delimiter_split(name_.c_str(), '_');

						if (box_info.size() < 2)
							Logger::

						nlohmann::json box_json = gamestate->get_child("index")->get_component<Component::Json>(box_info[0])->json[box_info[1]];

						auto box_x = Game::removed.x;
						auto box_y = Game::removed.y;
						float box_w = box_json["box_width"];
						float box_h = box_json["box_height"];

						float corner_size = box_json["corner_size"];
						float box_scale = box_json["box_scale"];

						float render_group = box_json["render_group"];

						auto scaled_corner_size = corner_size * box_scale;

						// box corners transforms
						auto& c_tl_corner_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x, box_y, scaled_corner_size, scaled_corner_size });
						auto& c_tr_corner_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x + box_w - scaled_corner_size, box_y, scaled_corner_size, scaled_corner_size });
						auto& c_bl_corner_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x, box_y + box_h - scaled_corner_size, scaled_corner_size, scaled_corner_size });
						auto& c_br_corner_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x + box_w - scaled_corner_size, box_y + box_h - scaled_corner_size, scaled_corner_size, scaled_corner_size });

						// box side transforms
						auto& c_t_side_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x + scaled_corner_size, box_y, box_w - scaled_corner_size * 2, scaled_corner_size });
						auto& c_l_side_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x, box_y + scaled_corner_size, scaled_corner_size, box_h - scaled_corner_size * 2 });
						auto& c_b_side_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x + scaled_corner_size, box_y + box_h - scaled_corner_size, box_w - scaled_corner_size * 2, scaled_corner_size });
						auto& c_r_side_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x + box_w - scaled_corner_size, box_y + scaled_corner_size, scaled_corner_size, box_h - scaled_corner_size * 2 });

						// center transform
						auto& c_center_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x + scaled_corner_size, box_y + scaled_corner_size, box_w - scaled_corner_size * 2, box_h - scaled_corner_size * 2 });

						// speech arrow
						auto& c_speech_arrow_trans = *entity_->push_back_component<Component::Transform>();

						// get textbox shader and texture

						// draw calls
						auto csr_tl_corner_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_tl_corner_src, c_tl_corner_trans, c_tb_material);
						auto csr_tr_corner_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_tr_corner_src, c_tr_corner_trans, c_tb_material);
						auto csr_bl_corner_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_bl_corner_src, c_bl_corner_trans, c_tb_material);
						auto csr_br_corner_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_br_corner_src, c_br_corner_trans, c_tb_material);
						auto csr_t_side_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_t_side_src, c_t_side_trans, c_tb_material);
						auto csr_l_side_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_l_side_src, c_l_side_trans, c_tb_material);
						auto csr_r_side_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_r_side_src, c_r_side_trans, c_tb_material);
						auto csr_b_side_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_b_side_src, c_b_side_trans, c_tb_material);
						auto csr_center_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_center_src, c_center_trans, c_tb_material);
						auto csr_speech_arrow_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_speech_arrow_src, c_speech_arrow_trans, c_tb_material);

						std::vector<Component::ISystem*> temp_systems;

						temp_systems.push_back(csr_tl_corner_camera_draw);
						temp_systems.push_back(csr_tr_corner_camera_draw);
						temp_systems.push_back(csr_bl_corner_camera_draw);
						temp_systems.push_back(csr_br_corner_camera_draw);
						temp_systems.push_back(csr_t_side_camera_draw);
						temp_systems.push_back(csr_l_side_camera_draw);
						temp_systems.push_back(csr_r_side_camera_draw);
						temp_systems.push_back(csr_b_side_camera_draw);
						temp_systems.push_back(csr_center_camera_draw);
						temp_systems.push_back(csr_speech_arrow_camera_draw);

						auto cs_item = e_game_info_->add_id_component<Component::System::IItem>("render_item", temp_systems);

						auto& render_engine = *gamestate->get_child("engine")->get_component<Component::Engine>("render");
						render_engine.add(cs_item, render_group);
					}

				};
			}
		}
	}
}