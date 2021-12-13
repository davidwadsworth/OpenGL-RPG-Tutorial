#pragma once
#include "component_vector.h"
#include "component_trigger_input.h"
#include "component_system.h"
#include <sstream>
#include "json.hpp"
#include "component_template.h"
#include "component_tree.h"
#include "component_material.h"
#include "component_system_render_camera_draw.h"
#include "component_controller_keyboard.h"
#include "component_src_bitmap_glyph.h"
#include "component_trigger_delete_game_obj.h"
#include "component_trigger_add_game_obj.h"
#include "component_trigger_delete_entity.h"
#include "component_trigger_switch_systems.h"
#include "component_observer.h"
#include "component_doubly_linked_list.h"

/*
@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace Dependent
			{
				class TextBox : public Component::Trigger::IInput
				{
					std::string path_;
					glm::vec2 pos_;
				public:
					TextBox(std::string name, std::string path, glm::vec2 pos)
						: Component::Trigger::IInput(name), path_(path), pos_(pos)
					{}

				private:
					void create(Entity* gamestate) override final
					{
						auto& c_tb_observer = *gamestate->get_child("observer")->add_id_component<Component::SystemObserver>("textbox");
						
			
						// load tilemap from file
						std::stringstream msg_stream;

						try
						{
							// open files
							std::ifstream msg_file(path_);

							// read into temp string streams
							msg_stream << msg_file.rdbuf();

							// close file streams
							msg_file.close();
						}
						catch (std::exception e)
						{
							Logger::error("Failed to read message file! path = " + path_, Logger::MEDIUM);
							return;
						}

						// parse into json obj
						auto msg_json = nlohmann::json::parse(msg_stream);

						// get relavent message info
						int box_h = msg_json["box_height"];
						int box_w = msg_json["box_width"];
						float box_sc = msg_json["box_scale"];
						std::string font_name = msg_json["font"];
						float font_sc = msg_json["font_scale"];
						std::string align_h = msg_json["align_horizontal"];
						std::string align_v = msg_json["align_vertical"];
						float line_spacing = msg_json["line_spacing"];
						float text_padding = msg_json["text_padding"];
						float msg_padding_x = msg_json["msg_padding_x"];
						float msg_padding_y = msg_json["msg_padding_y"];
						float corner_size = msg_json["corner_size"];
						bool speech_box = msg_json["speech_box"] == "true";
						std::string message = msg_json["message"];

						// get font
						auto font = gamestate->get_child("gilsans");

						// get renderer
						auto& c_renderer = *gamestate->get_child("renderer")->get_component<Component::Renderer>();

						// get controller 
						auto& c_cont_keyboard = *gamestate->get_child("controller")->get_component<Component::Controller::Keyboard>();

						// get camera
						auto& c_cam_transform = *gamestate->get_child("camera")->get_component<Component::Transform>();

						// get font material
						auto& c_font_material = *font->get_component<Component::Material>(0);

						// get textbox shader and texture
						auto& c_textbox_tex = *gamestate->get_child("texture manager")->get_component<Component::Texture>("flesh_full");
						auto& c_sprite_shader = *gamestate->get_child("shader manager")->get_component<Component::Shader>("sprite");

						// add textbox material
						auto& c_tb_material = *entity_->add_component<Component::Material>(c_textbox_tex, c_sprite_shader, 4);

						// create boxes

						auto line_h = font->get_component<Component::Integer>("line_h")->value;

						auto space = line_h / 3.0f;

						auto box_x = pos_.x - msg_padding_x;
						auto box_y = pos_.y - msg_padding_y - box_h;

						line_h = line_h - static_cast<int>(space / line_spacing);

						corner_size = corner_size * box_sc;
						text_padding = text_padding + corner_size;

						// event tree for textbox
						auto& c_system_tree = *entity_->push_back_component<Component::TriggerTree>();

						// box corners transforms
						auto& c_tl_corner_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x, box_y, corner_size, corner_size }, box_sc);
						auto& c_tr_corner_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x + box_w - corner_size, box_y, corner_size, corner_size }, box_sc);
						auto& c_bl_corner_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x, box_y + box_h - corner_size, corner_size, corner_size }, box_sc);
						auto& c_br_corner_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x + box_w - corner_size, box_y + box_h - corner_size, corner_size, corner_size }, box_sc);

						// box side transforms
						auto& c_t_side_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x + corner_size, box_y, box_w - corner_size * 2, corner_size }, box_sc);
						auto& c_l_side_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x, box_y + corner_size, corner_size, box_h - corner_size * 2 }, box_sc);
						auto& c_b_side_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x + corner_size, box_y + box_h - corner_size, box_w - corner_size * 2, corner_size }, box_sc);
						auto& c_r_side_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x + box_w - corner_size, box_y + corner_size, corner_size, box_h - corner_size * 2 }, box_sc);

						// center transform
						auto& c_center_trans = *entity_->push_back_component<Component::Transform>(
							Rect{ box_x + corner_size, box_y + corner_size, box_w - corner_size * 2, box_h - corner_size * 2 }, box_sc);

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
							Rect{ 10.0f, 0.0f, 10.0f, 10.0f });
						auto& c_r_side_src = *entity_->push_back_component<Component::Src>(
							Rect{ 20.0f, 10.0f, 10.0f, 10.0f });
						auto& c_bl_corner_src = *entity_->push_back_component<Component::Src>(
							Rect{ 0.0f, 20.0f, 10.0f, 10.0f });
						auto& c_b_side_src = *entity_->push_back_component<Component::Src>(
							Rect{ 10.0f, 20.0f, 10.0f, 10.0f });
						auto& c_br_corner_src = *entity_->push_back_component<Component::Src>(
							Rect{ 20.0f, 20.0f, 10.0f, 10.0f });


						auto csr_tl_corner_camera_draw = entity_->add_component<Component::System::Render::CameraDraw>(c_renderer, c_tl_corner_src, c_tl_corner_trans, c_tb_material, c_cam_transform);
						auto csr_tr_corner_camera_draw = entity_->add_component<Component::System::Render::CameraDraw>(c_renderer, c_tr_corner_src, c_tr_corner_trans, c_tb_material, c_cam_transform);
						auto csr_bl_corner_camera_draw = entity_->add_component<Component::System::Render::CameraDraw>(c_renderer, c_bl_corner_src, c_bl_corner_trans, c_tb_material, c_cam_transform);
						auto csr_br_corner_camera_draw = entity_->add_component<Component::System::Render::CameraDraw>(c_renderer, c_br_corner_src, c_br_corner_trans, c_tb_material, c_cam_transform);
						auto csr_t_side_camera_draw = entity_->add_component<Component::System::Render::CameraDraw>(c_renderer, c_t_side_src, c_t_side_trans, c_tb_material, c_cam_transform);
						auto csr_l_side_camera_draw = entity_->add_component<Component::System::Render::CameraDraw>(c_renderer, c_l_side_src, c_l_side_trans, c_tb_material, c_cam_transform);
						auto csr_r_side_camera_draw = entity_->add_component<Component::System::Render::CameraDraw>(c_renderer, c_r_side_src, c_r_side_trans, c_tb_material, c_cam_transform);
						auto csr_b_side_camera_draw = entity_->add_component<Component::System::Render::CameraDraw>(c_renderer, c_b_side_src, c_b_side_trans, c_tb_material, c_cam_transform);



						// arrow transform (used only with speech textboxes)
						if (speech_box)
						{
							auto& c_arrow_dest = *entity_->push_back_component<Component::Transform>();
							// TODO need to copy and paste old code from wayyyyy back repo project
						}

						auto current_pos = pos_ + text_padding;
						auto curr_char = message.begin();

						auto current_tb = new Entity();
						entity_->push_back_child(current_tb);

						std::vector<std::vector<Component::Transform*>> tb_lines;
						tb_lines.push_back(std::vector<Component::Transform*>());
						auto line_count = 0;
						std::vector<Component::Transform*>* curr_line = &tb_lines[line_count];

						do
						{
							// create line for textbox

							switch (*curr_char)
							{
							case ' ':
								current_pos.x += space;
								curr_char++;
								continue;
							case '[':
								// TODO generate option box
								break;
							case '\n':
								current_pos.y += line_h;
								current_pos.x = box_x + text_padding;

								if (current_pos.y > box_y + box_h - text_padding)
								{
									current_tb = new Entity();
									entity_->push_back_child(current_tb);

									tb_lines.clear();
									line_count = 0;
								}

								curr_char++;
								break;
							default:
								break;
							}

							std::vector<Component::Transform*> temp_transforms;
							std::vector<Component::BitMapGlyph*> temp_glyphs;
							auto temp_word_length = 0;

							GLint prev_char = -1;

							for (auto i = 0; *curr_char != ' ' && curr_char != message.end(); ++i)
							{
								auto c_bitmap_char = font->get_component<Component::BitMapGlyph>(message[i]);

								temp_word_length += c_bitmap_char->advance + c_bitmap_char->check_kerning(prev_char);
								prev_char = message[i];

								auto curr_x = current_pos.x + c_bitmap_char->x_off + c_bitmap_char->check_kerning(prev_char);
								auto curr_y = current_pos.y + c_bitmap_char->y_off;
								auto ch_w = c_bitmap_char->w;
								auto ch_h = c_bitmap_char->h;

								current_pos.x += c_bitmap_char->advance + c_bitmap_char->check_kerning(prev_char);

								auto cur_char_transform = entity_->push_back_component<Component::Transform>(
									Rect{ curr_x, curr_y, ch_w, ch_h }, font_sc);

								temp_transforms.push_back(cur_char_transform);
								temp_glyphs.push_back(c_bitmap_char);
							}

							if (temp_word_length + current_pos.x > box_x + box_w - text_padding)
							{
								current_pos.y += line_h;
								current_pos.x = box_x + text_padding;

								if (current_pos.y + line_h > box_y + box_h - text_padding)
								{
									current_tb = new Entity();
									entity_->push_back_child(current_tb);

									tb_lines.clear();
									line_count = 0;
								}
								tb_lines.push_back(std::vector<Component::Transform*>());
								curr_line = &tb_lines[++line_count];
							}

						} while (curr_char++ != message.end());

						auto tb_reference = new Entity();
						entity_->add_id_child(tb_reference, "reference");
						auto& c_tb_render_list = *tb_reference->add_id_component<Component::SystemList>("render systems");

						c_tb_render_list.push_back(csr_tl_corner_camera_draw);
						c_tb_render_list.push_back(csr_tr_corner_camera_draw);
						c_tb_render_list.push_back(csr_bl_corner_camera_draw);
						c_tb_render_list.push_back(csr_br_corner_camera_draw);
						c_tb_render_list.push_back(csr_t_side_camera_draw);
						c_tb_render_list.push_back(csr_l_side_camera_draw);
						c_tb_render_list.push_back(csr_r_side_camera_draw);
						c_tb_render_list.push_back(csr_b_side_camera_draw);

						auto& c_tb_creation_trigger_list = *tb_reference->add_id_component<Component::TriggerList>("creation triggers");
						

						auto e_pause = new Entity();
						auto& c_pause_render_vector = *e_pause->add_id_component<Component::SystemVector>("update");

						auto ct_tb_pause_update = entity_->push_back_component<Component::Trigger::SwitchSystems>();

						c_tb_creation_trigger_list.push_back(ct_tb_pause_update);

						// get the live triggers to be executed after game loop
						auto& c_engine_triggers = *gamestate->get_child("engine")->get_component<Component::TriggerVector>();
					}
				};
			}
		}
	}
}