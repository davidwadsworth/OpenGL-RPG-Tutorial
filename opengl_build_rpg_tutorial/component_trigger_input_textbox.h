#pragma once
#include "component_vector.h"
#include "component_trigger_input.h"
#include "component_system.h"
#include <sstream>
#include "json.hpp"
#include "component_template.h"
#include "component_tree.h"

/*
@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
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
					float padding = msg_json["padding"];
					int corner_size = msg_json["corner_size"];
					bool speech_box = msg_json["speech_box"] == "true";
					std::string message = msg_json["message"];
					
					// get font
					auto font = gamestate->get_child("gilsans");

					// create boxes

					// HelpersBox::generate

					auto line_h = font->get_component<Component::Integer>("line_h")->value;

					auto box_x = pos_.x;
					auto box_y = pos_.y;

					line_h = line_h - static_cast<int>(line_h / 3.0f / line_spacing);

					corner_size = corner_size * box_sc;
					padding = padding + corner_size;

					// event tree for textbox
					auto& c_system_tree = *entity_->push_back_component<Component::TriggerTree>();

					// box corners transforms
					auto& c_top_left_corner_dest = *entity_->push_back_component<Component::Transform>(
						Rect{ box_x, box_y, corner_size, corner_size }, box_sc);
					auto& c_top_right_corner_dest = *entity_->push_back_component<Component::Transform>(
						Rect{ box_x + box_w - corner_size, box_y, corner_size, corner_size }, box_sc);
					auto& c_bot_left_corner_dest = *entity_->push_back_component<Component::Transform>(
						Rect{ box_x, box_y + box_h - corner_size, corner_size, corner_size }, box_sc);
					auto& c_bot_right_corner_dest = *entity_->push_back_component<Component::Transform>(
						Rect{ box_x + box_w - corner_size, box_y + box_h - corner_size, corner_size, corner_size }, box_sc);

					// box side transforms
					auto& c_top_side_dest = *entity_->push_back_component<Component::Transform>(
						Rect{ box_x + corner_size, box_y, box_w - corner_size * 2, corner_size }, box_sc);
					auto& c_left_side_dest = *entity_->push_back_component<Component::Transform>(
						Rect{ box_x, box_y + corner_size, corner_size, box_h - corner_size * 2 }, box_sc);
					auto& c_bot_side_dest = *entity_->push_back_component<Component::Transform>(
						Rect{ box_x + corner_size, box_y + box_h - corner_size, box_w - corner_size * 2, corner_size }, box_sc);
					auto& c_right_side_dest = *entity_->push_back_component<Component::Transform>(
						Rect{ box_x + box_w - corner_size, box_y + corner_size, corner_size, box_h - corner_size * 2 }, box_sc);

					// center transform

					auto& c_center_dest = *entity_->push_back_component<Component::Transform>(
						Rect{ box_x + corner_size, box_y + corner_size, box_w - corner_size * 2, box_h - corner_size * 2 }, box_sc);


					// arrow transform (used only with speech textboxes)
					if (speech_box) auto lame = true;
					//auto& c_arrow_dest = *entity_->push_back_component<


					auto current_pos = pos_ + padding;
					auto curr_char = message.begin();
					
					Entity* current_tb = nullptr;
					
					do
					{
						// create a new textbox if needed
						if (!current_tb)
						{
							current_tb = new Entity();
							entity_->push_back_child(current_tb);
						}


						// create line for textbox

						switch (message[message_pos])
						{
						case ' ':
							current_pos.x += SPACE;
							++message_pos;
							continue;
						case '[':
							HelpersOptionBox::generate(box, font);
							break;
						case '\n':
							current_pos.y += line_h;
							current_pos.x = x + padding;

							if (current_pos.y > y + h - padding)
								return;

							++message_pos;
							break;
						case '|':
							++message_pos;
							return;
						default:
							break;
						}

						auto word = HelpersText::generate_word(&message[message_pos], ' ', '\0', font);

						if (word.length + current_pos.x > x + w - padding)
						{
							current_pos.y += line_h;
							current_pos.x = x + padding;

							if (current_pos.y + line_h > y + h - padding)
								return;
						}

						word.generate(current_pos, c_box, c_font_tex, box, font_scale);

						// HelpersText::generate_word

						temp_word;

						GLint prev_char = -1;

						auto c_font = font->get_component<Component::BitMapFont>();

						for (auto i = 0; message[i] != stop && message[i] != delimiter; ++i)
						{
							auto font_char = font->get_child(message[i] - c_font->child_off);
							temp_word.characters.push_back(font_char);

							auto c_bitmap_char = font_char->get_component<Component::BitMapGlyph>();

							temp_word.length += c_bitmap_char->advance + HelpersText::kerning(c_bitmap_char, prev_char);
							prev_char = message[i];
							temp_word.w.push_back(prev_char);


							// HelpersText::word.generate()
							auto c_bitmap_char = this->characters[i]->get_component<Component::BitMapGlyph>();
							auto& src = this->characters[i]->get_component<Component::Src>()->src;

							auto current_x1 = current_pos.x + c_bitmap_char->x_off + HelpersText::kerning(c_bitmap_char, prev_char);
							auto current_y1 = current_pos.y + c_bitmap_char->y_off;
							auto current_x2 = current_x1 + c_bitmap_char->width;
							auto current_y2 = current_y1 + c_bitmap_char->height;

							current_pos.x += c_bitmap_char->advance + HelpersText::kerning(c_bitmap_char, prev_char);

							auto cur_char = box->get_child(c_box->child_pos++);

							auto cur_char_src = cur_char->get_component<Component::Src>();
							cur_char_src->src = src;

							auto cur_char_dest = cur_char->get_component<Component::Dest>();
							cur_char_dest->dest = glm::vec4(current_x1, current_y1, current_x2, current_y2);
						}

						// check if needs a new textbox
						if (/*TODO*/0)
						{
							current_tb = nullptr;
						}


					} while (curr_char++ != message.end());
				}
			};
		}

	}
}