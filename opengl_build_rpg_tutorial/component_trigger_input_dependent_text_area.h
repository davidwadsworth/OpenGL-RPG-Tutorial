#pragma once
#include "component_vector.h"
#include "component_trigger_input.h"
#include "component_texture.h"
#include "component_shader.h"
#include "component_renderer.h"
#include "component_src_bitmap_glyph.h"
#include "component_system_render_draw.h"
#include "component_template.h"

/*
Display text to the screen without regard to the map.
 
@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace Dependent
			{
				class TextArea : public Component::Trigger::IInput
				{
					std::string& msg_;
					Rect rect_;
					std::string font_;
					float line_spacing_, font_sc_;
				public:
					TextArea(std::string name, std::string& msg, Rect rect, float line_spacing, float font_sc)
						: Component::Trigger::IInput(name), msg_(msg), rect_(rect), line_spacing_(line_spacing), font_sc_(font_sc)
					{}

				private:
					void create(Entity* gamestate) override final
					{
						// grab the input observer class for a helper method
						auto& cti_observer = *gamestate->get_component<Component::Trigger::Input::SystemObs>(0);

						// create an observer component for the box 
						auto e_observer = gamestate->get_child("observer");
						auto& c_box_observer = *e_observer->add_id_component<Component::SystemObserver>(name_);

						auto e_text_area_reference = new Entity();
						entity_->add_id_child(e_text_area_reference, "reference");
						auto& c_text_area_ref_render_list = *e_text_area_reference->add_id_component<Component::SystemList>("render systems");

						// get renderer
						auto& c_renderer = *gamestate->get_child("renderer")->get_component<Component::Renderer>();

						auto font = gamestate->get_child("gilsans");

						// hacky way to get font material, might change this, might not.
						auto& c_font_material = *font->get_component<Component::Color>();

						// create boxes

						auto line_h = font->get_component<Component::Integer>()->value;

						auto space = line_h / 3.0f;

						line_h = line_h - static_cast<int>(space / line_spacing_);


						// keeping track of transforms for alignment
						std::vector<std::vector<Component::Transform*>> tb_lines;
						tb_lines.push_back(std::vector<Component::Transform*>());

						auto line_count = 0;
						std::vector<Component::Transform*>* curr_line = &tb_lines[line_count];

						// count the messages for system tree construction
						auto msg_box_count = 0u;

						// the position where we are drawing characters on the screen
						auto current_pos = glm::vec2(rect_.x, rect_.y);

						// current character in message
						auto curr_char = msg_.begin();

						// skeleton for message box
						auto e_msg_box = new Entity();
						entity_->push_back_child(e_msg_box);

						std::vector<Component::System::Render::Draw*> temp_draws;
						do
						{
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
								current_pos.x = rect_.x;

								// if message character exceedes boundaries for box then create a new message to hold remainder
								if (current_pos.y > rect_.y + rect_.h)
									return;

								curr_char++;
								break;

							default:
								break;
							}

							std::vector<Component::Transform*> temp_transforms;
							std::vector<Component::BitMapGlyph*> temp_glyphs;

							auto e_glyphs = font->get_child("glyphs");

							auto temp_word_length = 0;

							GLint prev_char = -1;

							// add all the non space characters together into vectors of transforms glyphs and draw calls, also known as a "word"
							for (; curr_char != msg_.end() && *curr_char != ' '; curr_char++)
							{
								auto c_bitmap_char = e_glyphs->get_component<Component::BitMapGlyph>(static_cast<std::size_t>( * curr_char));

								temp_word_length += c_bitmap_char->advance + c_bitmap_char->check_kerning(prev_char);
								
								auto curr_x = current_pos.x + c_bitmap_char->x_off + c_bitmap_char->check_kerning(prev_char);
								auto curr_y = current_pos.y + c_bitmap_char->y_off;
								auto ch_w = c_bitmap_char->w;
								auto ch_h = c_bitmap_char->h;

								current_pos.x += c_bitmap_char->advance + c_bitmap_char->check_kerning(prev_char);
								
								prev_char = *curr_char;

								auto c_cur_char_transform = entity_->push_back_component<Component::Transform>(
									Rect{ curr_x, curr_y, ch_w, ch_h }, font_sc_);

								temp_transforms.push_back(c_cur_char_transform);
								temp_glyphs.push_back(c_bitmap_char);

								auto csr_msg_draw = e_msg_box->push_back_component<Component::System::Render::Draw>(c_renderer, *c_bitmap_char, *c_cur_char_transform, c_font_material);
								//auto n_msg_cam_draw = c_text_area_ref_render_list.push_back(csr_msg_cam_draw);
								temp_draws.push_back(csr_msg_draw);
								
							}

							// if the added word breaks the x boundaries of the box create a new line
							if (temp_word_length + current_pos.x > rect_.x + rect_.w)
							{
								current_pos.y += line_h;
								current_pos.x = rect_.x;

								// if the added line breaks the y boundary of the box create a new text box
								if (current_pos.y + line_h > rect_.y + rect_.h)
									return;

								tb_lines.push_back(std::vector<Component::Transform*>());
								curr_line = &tb_lines[++line_count];

							}

						} while (curr_char != msg_.end());



						//cti_observer.add_observed(std::vector<>{ n_msg_cam_draw }, std::vector<std::string>{ "camera", "renderer", "gilsans", name_ });


						auto& c_render_systems = *gamestate->get_child("engine")->get_component<Component::SystemVector>("render");
						
						c_render_systems.insert(c_render_systems.end(), temp_draws.begin(), temp_draws.end());

					}
				};
			}
		}

	}
}

