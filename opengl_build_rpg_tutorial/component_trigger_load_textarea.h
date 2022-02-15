#pragma once
#include "component_trigger.h"
#include "component_transform.h"
#include "component_template.h"
#include <glm/glm.hpp>
#include "component_src_bitmap_glyph.h"
#include "component_system_item_empty.h"
#include "component_trigger_load.h"

namespace Component {
	namespace Trigger {
		namespace Load
		{
			class TextArea : public Component::Trigger::ILoad
			{
			public:
				void execute(Entity* gamestate) override
				{
					if (!json_)
						Logger::error("missing load call to textarea", Logger::SEVERITY::HIGH);
					
					// create boxes
					auto line_h = font_->get_component<Component::Integer>()->value;

					auto space = line_h / 3.0f;

					line_h = line_h - static_cast<int>(space / line_spacing_);

					// keeping track of transforms for alignment
					std::vector<std::vector<Component::Transform*>> tb_lines;
					tb_lines.push_back(std::vector<Component::Transform*>());

					auto line_count = 0;
					std::vector<Component::Transform*>* curr_line = &tb_lines[line_count];

					// the position where we are drawing characters on the screen
					auto current_pos = glm::vec2(rect_.x, rect_.y);

					// current character in message
					auto curr_char = msg_.begin();
					auto msg_i = 0;

					while (curr_char != msg_.end())
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
							if (current_pos.y + 2 * line_h > rect_.y + rect_.h)
								goto end;

							current_pos.y += line_h;
							current_pos.x = rect_.x;

							curr_char++;

							tb_lines.push_back(std::vector<Component::Transform*>());
							curr_line = &tb_lines[++line_count];
							break;
						case '\e':

						default:
							break;
						}

						std::vector<Component::Transform*> temp_transforms;

						auto e_glyphs = font_->get_child("glyphs");

						auto temp_word_length = 0;

						GLint prev_char = -1;
						float prev_pos_x = current_pos.x;

						// add all the non space characters together into vectors of transforms glyphs and draw calls, also known as a "word"
						for (; curr_char != msg_.end() && *curr_char != ' '; curr_char++)
						{
							auto& c_bitmap_char = *e_glyphs->get_component<Component::BitMapGlyph>(static_cast<std::size_t>(*curr_char));

							temp_word_length += c_bitmap_char.advance + c_bitmap_char.check_kerning(prev_char);

							auto& c_cur_char_src = *textarea_->get_component<Component::Src>(msg_i * 3);

							c_cur_char_src.x = c_bitmap_char.x;
							c_cur_char_src.y = c_bitmap_char.y;
							c_cur_char_src.w = c_bitmap_char.w;
							c_cur_char_src.h = c_bitmap_char.h;

							auto c_cur_char_transform = textarea_->get_component<Component::Transform>(msg_i++ * 3 + 1);

							c_cur_char_transform->x = current_pos.x + c_bitmap_char.x_off + c_bitmap_char.check_kerning(prev_char);
							c_cur_char_transform->y = current_pos.y + c_bitmap_char.y_off;
							c_cur_char_transform->w = c_bitmap_char.w;
							c_cur_char_transform->h = c_bitmap_char.h;
							c_cur_char_transform->sc = font_sc_;

							current_pos.x += c_bitmap_char.advance + c_bitmap_char.check_kerning(prev_char);

							prev_char = *curr_char;

							temp_transforms.push_back(c_cur_char_transform);
						}

						// if the added word breaks the x boundaries of the box create a new line
						if (temp_word_length + prev_pos_x > rect_.x + rect_.w)
						{
							// if the added line breaks the y boundary of the box create a new text box
							if (current_pos.y + 2 * line_h > rect_.y + rect_.h)
								goto end;

							auto begin_tranform_x = temp_transforms[0]->x;

							for (auto transform : temp_transforms)
							{
								transform->x -= begin_tranform_x - rect_.x;
								transform->y += line_h;
							}

							current_pos.y += line_h;
							current_pos.x = rect_.x + temp_word_length;

							tb_lines.push_back(std::vector<Component::Transform*>());
							curr_line = &tb_lines[++line_count];
						}

						curr_line->insert(curr_line->end(), temp_transforms.begin(), temp_transforms.end());

					}

				end:
					auto x_offset = std::vector<float>(tb_lines.size(), 0.0f); // align left
					auto y_offset = 0.0f; // align top

					for (auto i = 0; i < tb_lines.size(); ++i)
					{
						auto line_segment = rect_.w - (((*(tb_lines[i].end() - 1))->x + (*(tb_lines[i].end() - 1))->w) - (*tb_lines[i].begin())->x);

						if (align_h_ == "middle")
							x_offset[i] = line_segment / 2.0f;
						else if (align_h_ == "right")
							x_offset[i] = line_segment;
					}

					auto y_lowest = FLT_MAX;
					auto y_highest = -FLT_MAX;

					for (auto transform : *tb_lines.begin())
						if (transform->y < y_lowest)
							y_lowest = transform->y;

					for (auto transform : *(tb_lines.end() - 1))
						if (transform->y + transform->h > y_highest)
							y_highest = transform->y + transform->h;

					if (y_lowest == FLT_MAX)
						y_lowest = rect_.y;
					if (y_highest == -FLT_MAX)
						y_highest = rect_.y + rect_.h;

					if (align_v_ == "middle")
						y_offset = (rect_.h - (y_highest - y_lowest)) / 2.0f;
					else if (align_v_ == "bottom")
						y_offset = (rect_.h - (y_highest - y_lowest));

					for (auto i = 0; i < tb_lines.size(); ++i)
						for (auto transform : tb_lines[i])
						{
							transform->x += x_offset[i];
							transform->y += y_offset;
						}
					textarea_->get_component<Component::System::Item::Empty>("render")->set_draw_calls(msg_.size());
					json_ = nullptr;
				}
			};
		}
	}
}