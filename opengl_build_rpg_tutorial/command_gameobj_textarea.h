#pragma once
#include "command.h"
#include "component_vector.h"
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
#include "game.h"

/*
Display text on screen defined to an area using common text writing rules
json string to create a textarea.

{
	"rect" : {"x" : ..., "y" : ..., "w" : ..., "h" : ...}, // the sizing and position of the textarea
	"message" : "...",
	"font" : "...", // name of the font used by the gamestate
	"font_scale" : ...,
	"align_horizontal" : 0.0 -> 1.0 // text alignment horizonal
	"align_vertical" : 0.0 -> 1.0 // text alignment vertical
	"line_spacing" : ... // the spacing value * line_h which is defined by the font
	"msg_padding" : {"x" : ..., "y" : ...}, // padding for the text on the top bottom and sides 
	"render_group" : ... // where in renderer is it drawn to
	"update_group" : ... // where in the update group is it updated
}
 
@author David Wadsworth
*/

namespace Command {
	namespace GameObj
	{
		class TextArea : public ICommand
		{

			float x_;
			float y_;
			float w_;
			float h_;


			std::string msg_;
			std::string font_name_;
			std::string name_;
			float font_sc_;
			std::string align_h_;
			std::string align_v_;
			float line_spacing_;
			float padding_x_;
			float padding_y_;
			float render_group_;
			float update_group_;

		public:
			void load(nlohmann::json json) override final
			{
				x_ = json["rect"]["x"];
				y_ = json["rect"]["y"];
				w_ = json["rect"]["w"];
				h_ = json["rect"]["h"];


				msg_ = json["message"];
				font_name_ = json["font"];
				font_sc_ = json["font_scale"];
				align_h_ = json["align_horizontal"];
				align_v_ = json["align_vertical"];
				line_spacing_ = json["line_spacing"];
				padding_x_ = json["msg_padding"]["x"];
				padding_y_ = json["msg_padding"]["y"];
				render_group_ = json["render_group"];
				update_group_ = json["update_group"];
			}

			void execute(Entity* gamestate) override final
			{
				auto e_textarea = gamestate->add_id_child(name_);

				// get renderer
				auto& c_renderer = *gamestate->get_component<Component::Renderer>();

				auto e_font = gamestate->get_child(font_name_);

				auto& c_font_material = *e_font->get_component<Component::Material>("material");

				// create boxes
				auto line_h = e_font->get_component<Component::Integer>("line_h")->value;

				auto space = line_h / 3.0f;

				line_h = line_h - static_cast<int>(space / line_spacing_);

				auto& c_position = *e_textarea->add_component<Component::Position>(Game::removed);

				// keeping track of transforms for alignment
				std::vector<std::vector<Component::Transform*>> tb_lines;
				tb_lines.push_back(std::vector<Component::Transform*>());

				auto line_count = 0;
				std::vector<Component::Transform*>* curr_line = &tb_lines[line_count];

				// the position where we are drawing characters on the screen
				auto current_pos = glm::vec2(x_, y_);

				// current character in message
				auto curr_char = msg_.begin();

				// skeleton for message box
				auto e_msg_box = new Entity();
				e_textarea->push_back_child(e_msg_box);

				std::vector<Component::Src*> src_vec;
				do
				{
					switch (*curr_char)
					{
					case ' ':
						current_pos.x += space;
						curr_char++;
						continue;
					case '\n':
						// if message character exceedes boundaries for box then create a new message to hold remainder
						if (current_pos.y + 2 * line_h > y_ + h_)
							goto end;

						current_pos.y += line_h;
						current_pos.x = x_;

						curr_char++;

						tb_lines.push_back(std::vector<Component::Transform*>());
						curr_line = &tb_lines[++line_count];
						break;

					default:
						break;
					}

					std::vector<Component::Transform*> line_transforms;
					std::vector<Component::Rectangle::BitMapGlyph*> line_glyphs;

					auto e_glyphs = e_font->get_child("glyphs");

					auto temp_word_length = 0;

					GLint prev_char = -1;
					float prev_pos_x = current_pos.x;

					// add all the non space characters together into vectors of transforms glyphs and draw calls, also known as a "word"
					for (; curr_char != msg_.end() && *curr_char != ' '; curr_char++)
					{
						auto c_bitmap_char = e_glyphs->get_component<Component::Rectangle::BitMapGlyph>(static_cast<std::size_t>( * curr_char));

						temp_word_length += c_bitmap_char->advance + c_bitmap_char->check_kerning(prev_char);
								
						auto curr_x = current_pos.x + c_bitmap_char->x_off + c_bitmap_char->check_kerning(prev_char);
						auto curr_y = current_pos.y + c_bitmap_char->y_off;
						auto ch_w = c_bitmap_char->w;
						auto ch_h = c_bitmap_char->h;

						current_pos.x += c_bitmap_char->advance + c_bitmap_char->check_kerning(prev_char);
								
						prev_char = *curr_char;

						auto c_cur_char_transform = e_textarea->push_back_component<Component::Transform>(
							Rect( curr_x, curr_y, ch_w, ch_h ) * font_sc_ );

						line_transforms.push_back(c_cur_char_transform);
						line_glyphs.push_back(c_bitmap_char);
					}

					// if the added word breaks the x boundaries of the box create a new line
					if (temp_word_length + prev_pos_x > x_ + w_)
					{								
						// if the added line breaks the y boundary of the box create a new text box
						if (current_pos.y + 2 * line_h > y_ + h_)
							goto end;

						auto begin_tranform_x = line_transforms[0]->x;

						for (auto transform : line_transforms)
						{
							transform->x -= begin_tranform_x - x_;
							transform->y += line_h;
						}

						current_pos.y += line_h;
						current_pos.x = x_ + temp_word_length;

						tb_lines.push_back(std::vector<Component::Transform*>());
						curr_line = &tb_lines[++line_count];
					}

					curr_line->insert(curr_line->end(), line_transforms.begin(), line_transforms.end());

				} while (curr_char != msg_.end());

			end:
				auto x_offset = std::vector<float>(tb_lines.size(), 0.0f); // align left
				auto y_offset = 0.0f; // align top

				for (auto i = 0; i < tb_lines.size(); ++i)
				{
					auto line_segment = w_ - (((*(tb_lines[i].end()-1))->x + (*(tb_lines[i].end()-1))->w) - (*tb_lines[i].begin())->x);

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
						
				for (auto transform : *(tb_lines.end()-1))
					if (transform->y + transform->h > y_highest)
						y_highest = transform->y + transform->h;
						
				if (y_lowest == FLT_MAX)
					y_lowest = y_;
				if (y_highest == -FLT_MAX)
					y_highest = y_ + h_;

				if (align_v_ == "middle")
					y_offset = (h_ - (y_highest - y_lowest)) / 2.0f;
				else if (align_v_ == "bottom")
					y_offset = (h_ - (y_highest - y_lowest));

				std::vector < Component::Transform* > transform_vec;
				for (auto i = 0; i < tb_lines.size(); ++i)
					for (auto transform : tb_lines[i])
					{
						transform->x += x_offset[i];
						transform->y += y_offset;
						transform_vec.push_back(transform);
					}

				auto csr_offset_render = e_textarea->add_id_component<Component::System::Render::Offset>("render", c_position, src_vec, transform_vec, c_font_material, c_renderer);
				gamestate->get_child("engine")->get_component<Component::Engine>("render")->add(csr_offset_render, render_group_);
			}
		};
	}
}

