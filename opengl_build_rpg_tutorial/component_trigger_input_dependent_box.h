#pragma once
#include "component_trigger_input.h"
#include "component_transform.h"
#include "rect.h"
#include <glm/vec2.hpp>
#include "component_observer.h"
#include "component_src.h"
#include "component_material.h"
#include "component_system_render_camera_draw.h"
#include "component_doubly_linked_list.h"
#include "component_trigger_input_observer.h"

/*
Creates our local camera using width and height

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace Dependent
			{
				class Box : public Component::Trigger::IInput
				{
					Rect rect_;
					float corner_size_, box_sc_;
					bool speech_arrow_;
				public:
					Box(std::string name, Rect rect, float corner_size, float box_sc, bool speech_arrow = false)
						: Component::Trigger::IInput(name), rect_(rect), corner_size_(corner_size), box_sc_(box_sc), speech_arrow_(speech_arrow)
					{}
				private:
					void create(Entity* gamestate) override final
					{
						// grab the input observer class for a helper method
						auto& cti_observer = *gamestate->get_component<Component::Trigger::Input::SystemObs>(0);

						// create an observer component for the box 
						cti_observer.add_subscriber(name_);

						auto e_box_reference = new Entity();
						entity_->add_id_child(e_box_reference, "reference");
						auto& c_box_ref_render_list = *e_box_reference->add_id_component<Component::SystemList>("render systems");

						// get renderer
						auto& c_renderer = *gamestate->get_child("renderer")->get_component<Component::Renderer>();

						// get camera
						auto& c_cam_transform = *gamestate->get_child("camera")->get_component<Component::Transform>();

						// get textbox shader and texture
						auto& c_textbox_tex = *gamestate->get_child("texture manager")->get_component<Component::Texture>("textbox");
						auto& c_sprite_shader = *gamestate->get_child("shader manager")->get_component<Component::Shader>("sprite");

						// add textbox material
						auto& c_tb_material = *entity_->push_back_component<Component::Material>(c_textbox_tex, c_sprite_shader, 4);

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

						// once more apropriate sprite sheets come into play these will be moved out into a different entity
						auto& c_tl_corner_src = *entity_->push_back_component<Component::Src>(
							Rect{ 0.0f, 0.0f, 10.0f, 10.0f });
						auto& c_t_side_src = *entity_->push_back_component<Component::Src>(
							Rect{ 10.0f, 0.0f, 10.0f, 10.0f });
						auto& c_tr_corner_src = *entity_->push_back_component<Component::Src>(
							Rect{ 20.0f, 0.0f, 10.0f, 10.0f });
						auto& c_l_side_src = *entity_->push_back_component<Component::Src>(
							Rect{ 0.0f, 10.0f, 10.0f, 10.0f });
						auto& c_center_src = *entity_->push_back_component<Component::Src>(
							Rect{ 10.0f, 10.0f, 10.0f, 10.0f });
						auto& c_r_side_src = *entity_->push_back_component<Component::Src>(
							Rect{ 20.0f, 10.0f, 10.0f, 10.0f });
						auto& c_bl_corner_src = *entity_->push_back_component<Component::Src>(
							Rect{ 0.0f, 20.0f, 10.0f, 10.0f });
						auto& c_b_side_src = *entity_->push_back_component<Component::Src>(
							Rect{ 10.0f, 20.0f, 10.0f, 10.0f });
						auto& c_br_corner_src = *entity_->push_back_component<Component::Src>(
							Rect{ 20.0f, 20.0f, 10.0f, 10.0f });

						// push back some systems
						auto csr_tl_corner_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_tl_corner_src, c_tl_corner_trans, c_tb_material);
						auto csr_tr_corner_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_tr_corner_src, c_tr_corner_trans, c_tb_material);
						auto csr_bl_corner_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_bl_corner_src, c_bl_corner_trans, c_tb_material);
						auto csr_br_corner_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_br_corner_src, c_br_corner_trans, c_tb_material);
						auto csr_t_side_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_t_side_src, c_t_side_trans, c_tb_material);
						auto csr_l_side_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_l_side_src, c_l_side_trans, c_tb_material);
						auto csr_r_side_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_r_side_src, c_r_side_trans, c_tb_material);
						auto csr_b_side_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_b_side_src, c_b_side_trans, c_tb_material);
						auto csr_center_camera_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_center_src, c_center_trans, c_tb_material);
						
						/*
						std::vector<DoublyLinkedList<ISystem*>::Node*> used_tb_nodes;

						used_tb_nodes.push_back(c_box_ref_render_list.push_back(csr_tl_corner_camera_draw));
						used_tb_nodes.push_back(c_box_ref_render_list.push_back(csr_tr_corner_camera_draw));
						used_tb_nodes.push_back(c_box_ref_render_list.push_back(csr_bl_corner_camera_draw));
						used_tb_nodes.push_back(c_box_ref_render_list.push_back(csr_br_corner_camera_draw));
						used_tb_nodes.push_back(c_box_ref_render_list.push_back(csr_t_side_camera_draw));
						used_tb_nodes.push_back(c_box_ref_render_list.push_back(csr_l_side_camera_draw));
						used_tb_nodes.push_back(c_box_ref_render_list.push_back(csr_r_side_camera_draw));
						used_tb_nodes.push_back(c_box_ref_render_list.push_back(csr_b_side_camera_draw));
						used_tb_nodes.push_back(c_box_ref_render_list.push_back(csr_center_camera_draw));
						*/

						//cti_observer.add_observed(used_tb_nodes, std::vector<std::string>{"camera", "renderer", name_});

						auto& c_render_systems = *gamestate->get_child("engine")->get_component<Component::SystemVector>("render");

						c_render_systems.push_back(csr_tl_corner_camera_draw);
						c_render_systems.push_back(csr_tr_corner_camera_draw);
						c_render_systems.push_back(csr_bl_corner_camera_draw);
						c_render_systems.push_back(csr_br_corner_camera_draw);
						c_render_systems.push_back(csr_t_side_camera_draw);
						c_render_systems.push_back(csr_l_side_camera_draw);
						c_render_systems.push_back(csr_r_side_camera_draw);
						c_render_systems.push_back(csr_b_side_camera_draw);
						c_render_systems.push_back(csr_center_camera_draw);

						// arrow transform (used only with speech textboxes)
						if (speech_arrow_)
						{
							auto& c_arrow_dest = *entity_->push_back_component<Component::Transform>();
							// TODO need to copy and paste old code from wayyyyy back repo project
						}

					}

				};
			}
		}
	}
}