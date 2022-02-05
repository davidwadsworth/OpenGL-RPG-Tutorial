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
					Rect rect_;
					float corner_size_, box_sc_;
					std::size_t render_group_;
				public:
					Box(std::string name, std::size_t render_group)
						: Component::Trigger::Input::IGameObj(name), rect_{Game::removed.x, Game::removed.y, 0.0f, 0.0f}, corner_size_(0.0f), box_sc_(0.0f), 
						render_group_(render_group)
					{}

				private:
					void init(Entity* gamestate) override final
					{
						auto box_x = rect_.x;
						auto box_y = rect_.y;
						auto box_w = rect_.w;
						auto box_h = rect_.h;

						auto scaled_corner_size = corner_size_ * box_sc_;

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


						// once more apropriate sprite sheets come into play these will be moved out into a different entity
						auto& c_tl_corner_src = *entity_->push_back_component<Component::Src>(
							Rect{ 0.0f, 0.0f, corner_size_, corner_size_ });
						auto& c_t_side_src = *entity_->push_back_component<Component::Src>(
							Rect{ corner_size_, 0.0f, corner_size_, corner_size_ });
						auto& c_tr_corner_src = *entity_->push_back_component<Component::Src>(
							Rect{ corner_size_ * 2.0f, 0.0f, corner_size_, corner_size_ });
						auto& c_l_side_src = *entity_->push_back_component<Component::Src>(
							Rect{ 0.0f, corner_size_, corner_size_, corner_size_ });
						auto& c_center_src = *entity_->push_back_component<Component::Src>(
							Rect{ corner_size_, corner_size_, corner_size_, corner_size_ });
						auto& c_r_side_src = *entity_->push_back_component<Component::Src>(
							Rect{ corner_size_ * 2.0f, corner_size_, corner_size_, corner_size_ });
						auto& c_bl_corner_src = *entity_->push_back_component<Component::Src>(
							Rect{ 0.0f, corner_size_ * 2.0f , corner_size_, corner_size_ });
						auto& c_b_side_src = *entity_->push_back_component<Component::Src>(
							Rect{ corner_size_, corner_size_ * 2.0f, corner_size_, corner_size_ });
						auto& c_br_corner_src = *entity_->push_back_component<Component::Src>(
							Rect{ corner_size_ * 2.0f, corner_size_ * 2.0f, corner_size_, corner_size_ });
						auto& c_speech_arrow_src = *entity_->push_back_component<Component::Src>(
							Rect{0.0f, corner_size_ * 3.0f, corner_size_, corner_size_ });

						auto& c_renderer = *gamestate->get_child("engine")->get_component<Component::Renderer>("renderer");
						// get textbox shader and texture
						auto& c_textbox_tex = *gamestate->get_child("texture manager")->get_component<Component::Texture>("textbox");
						auto& c_sprite_shader = *gamestate->get_child("shader manager")->get_component<Component::Shader>("sprite");

						// add textbox material
						auto& c_tb_material = *entity_->push_back_component<Component::Material>(c_textbox_tex, c_sprite_shader, 4);

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
						render_engine.add(cs_item, render_group_);
					}

				};
			}
		}
	}
}