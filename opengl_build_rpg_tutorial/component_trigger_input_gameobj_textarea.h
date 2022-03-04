#pragma once
#include "component_vector.h"
#include "component_trigger_input.h"
#include "component_texture.h"
#include "component_shader.h"
#include "component_renderer.h"
#include "component_rect_bitmap_glyph.h"
#include "component_system_render_draw.h"
#include "component_template.h"
#include "component_material_color.h"
#include "component_engine.h"
#include "delimiter_split.h"
#include "component_json.h"
#include "component_system_render_offset.h"

/*
Display text on screen defined to an area using common text writing rules and 
 
@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace GameObj
			{
				class TextArea : public Component::Trigger::IInput
				{
				private:
					void create(Entity* gamestate) override final
					{
						auto textarea_name = delimiter_split(name_.c_str(), '_')[0];

						auto textarea_json = gamestate->get_child("index")->get_component<Component::Json>(textarea_name)->json;

						float x = textarea_json["rect"]["x"];
						float y = textarea_json["rect"]["y"];
						float w = textarea_json["rect"]["w"];
						float h = textarea_json["rect"]["h"];
						std::string msg = textarea_json["message"];
						std::string font_name = textarea_json["font"];
						float font_sc = textarea_json["font_scale"];
						std::string align_h = textarea_json["align_horizontal"];
						std::string align_v = textarea_json["align_vertical"];
						float line_spacing = textarea_json["line_spacing"];
						float padding_x = textarea_json["msg_padding"]["x"];
						float padding_y = textarea_json["msg_padding"]["y"];
						float render_group = textarea_json["render_group"];
						float update_group = textarea_json["update_group"];
						// get renderer
						auto& c_renderer = *gamestate->get_component<Component::Renderer>();

						auto e_font = gamestate->get_child(font_name);

						auto& c_font_material = *e_font->get_component<Component::Material>("material");

						// create boxes
						auto line_h = e_font->get_component<Component::Integer>("line_h")->value;

						auto space = line_h / 3.0f;

						line_h = line_h - static_cast<int>(space / line_spacing);

						auto& c_position = *entity_->add_component<Component::Position>(Game::removed);

						// keeping track of transforms for alignment
						std::vector<std::vector<Component::Transform*>> tb_lines;
						tb_lines.push_back(std::vector<Component::Transform*>());

						auto line_count = 0;
						std::vector<Component::Transform*>* curr_line = &tb_lines[line_count];

						// the position where we are drawing characters on the screen
						auto current_pos = glm::vec2(x, y);

						// current character in message
						auto curr_char = msg.begin();

						// skeleton for message box
						auto e_msg_box = new Entity();
						entity_->push_back_child(e_msg_box);

						std::vector<Component::Src*> src_vec;
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
								// if message character exceedes boundaries for box then create a new message to hold remainder
								if (current_pos.y + 2 * line_h > y + h)
									goto end;

								current_pos.y += line_h;
								current_pos.x = x;

								curr_char++;

								tb_lines.push_back(std::vector<Component::Transform*>());
								curr_line = &tb_lines[++line_count];
								break;

							default:
								break;
							}

							std::vector<Component::Transform*> line_transforms;
							std::vector<Component::BitMapGlyph*> line_glyphs;

							auto e_glyphs = e_font->get_child("glyphs");

							auto temp_word_length = 0;

							GLint prev_char = -1;
							float prev_pos_x = current_pos.x;

							// add all the non space characters together into vectors of transforms glyphs and draw calls, also known as a "word"
							for (; curr_char != msg.end() && *curr_char != ' '; curr_char++)
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
									Rect( curr_x, curr_y, ch_w, ch_h ) * font_sc );

								line_transforms.push_back(c_cur_char_transform);
								line_glyphs.push_back(c_bitmap_char);
							}

							// if the added word breaks the x boundaries of the box create a new line
							if (temp_word_length + prev_pos_x > x + w)
							{								
								// if the added line breaks the y boundary of the box create a new text box
								if (current_pos.y + 2 * line_h > y + h)
									goto end;

								auto begin_tranform_x = line_transforms[0]->x;

								for (auto transform : line_transforms)
								{
									transform->x -= begin_tranform_x - x;
									transform->y += line_h;
								}

								current_pos.y += line_h;
								current_pos.x = x + temp_word_length;

								tb_lines.push_back(std::vector<Component::Transform*>());
								curr_line = &tb_lines[++line_count];
							}

							curr_line->insert(curr_line->end(), line_transforms.begin(), line_transforms.end());

						} while (curr_char != msg.end());

					end:
						auto x_offset = std::vector<float>(tb_lines.size(), 0.0f); // align left
						auto y_offset = 0.0f; // align top

						for (auto i = 0; i < tb_lines.size(); ++i)
						{
							auto line_segment = w - (((*(tb_lines[i].end()-1))->x + (*(tb_lines[i].end()-1))->w) - (*tb_lines[i].begin())->x);

							if (align_h == "middle")
								x_offset[i] = line_segment / 2.0f;
							else if (align_h == "right")
								x_offset[i] = line_segment;
						}

						auto y_lowest = FLT_MAX;
						auto y_highest = -FLT_MAX;

						for (auto transform : *tb_lines.begin())
							if (transform->y < y_lowest)
								y_lowest = transform->y;
						
						for (auto transform : *(tb_lines.end()-1))
							if (transform->y + transform->h > y_highest)
								y_highest = transform->y + transform->h;
						
						if (y_lowest == FLT_MAX)
							y_lowest = y;
						if (y_highest == -FLT_MAX)
							y_highest = y + h;

						if (align_v == "middle")
							y_offset = (h - (y_highest - y_lowest)) / 2.0f;
						else if (align_v == "bottom")
							y_offset = (h - (y_highest - y_lowest));

						std::vector < Component::Transform* > transform_vec;
						for (auto i = 0; i < tb_lines.size(); ++i)
							for (auto transform : tb_lines[i])
							{
								transform->x += x_offset[i];
								transform->y += y_offset;
								transform_vec.push_back(transform);
							}

						auto csr_offset_render = entity_->add_id_component<Component::System::Render::Offset>("render", c_position, src_vec, transform_vec, c_font_material, c_renderer);
						gamestate->get_child("engine")->get_component<Component::Engine>("render")->add(csr_offset_render, render_group);
					}
				public:
					using Component::Trigger::IInput::IInput;
				};
			}
		}

	}
}

