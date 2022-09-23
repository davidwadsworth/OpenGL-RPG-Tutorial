#pragma once
#include "load.h"
#include "component_system_render_blockdraw.h"
#include "component_cursor.h"

namespace Load
{
	class OptionBox : public ILoad
	{

		Rect rect_;
		std::string font_name_, align_h_, align_v_, textbox_name_, nav_;
		float font_sc_, render_group_, pos_x_, pos_y_;
		nlohmann::json cursor_json_;
		std::vector<std::string> options_;
		float padding_;
	public:

		void load(nlohmann::json json)
		{
			align_h_ = json["textbox"]["optionbox"]["align_horizontal"];
			align_v_ = json["textbox"]["optionbox"]["align_vertical"];
			int box_w = json["textbox"]["box"]["width"];
			int box_h = json["textbox"]["box"]["height"];
			float padding_x = json["textarea"]["textarea"]["msg_padding"]["x"];
			float padding_y = json["textarea"]["textarea"]["msg_padding"]["y"];

			pos_x_ = json["textbox"]["optionbox"]["pos_x"];
			pos_y_ = json["textbox"]["optionbox"]["pos_y"];

			pos_x_ = pos_x_ * box_w - padding_x;
			pos_y_ = pos_y_ * box_h - padding_y;

			nav_ = json["textbox"]["optionbox"]["nav"];
			cursor_json_ = json["textbox"]["optionbox"]["cursor"];
			textbox_name_ = json["textbox"]["name"];
			font_name_ = json["textbox"]["textarea"]["font"];
			font_sc_ = json["textbox"]["textarea"]["font_scale"];
			options_ = json["load"]["data"]["options"].get<std::vector<std::string>>();
		}

		void execute(Entity* gamestate) override
		{
			auto& csr_block_draw = *gamestate->get_child(textbox_name_)->get_component<Component::System::Render::BlockDraw>("render");
			
			auto char_count = 0;

			for (auto option : options_)
				char_count += option.size();

			auto option_blocks = csr_block_draw.get_blocks(char_count);
			auto cursor_blocks = csr_block_draw.get_blocks(options_.size());

			auto e_font = gamestate->get_child(font_name_);
			auto c_font_material = e_font->get_component<Component::Material>("material");

			csr_block_draw.add_render_group(option_blocks, c_font_material);

			auto e_spritesheet = gamestate->get_child(cursor_json_["spritesheet"].get<std::string>());
			auto c_spritesheet_material = e_spritesheet->get_component<Component::Material>("material");

			csr_block_draw.add_render_group(cursor_blocks, c_spritesheet_material);

			// iterate through options and find the src and dest dimensions for the text assume that it starts at 0,0
			// store those messages into blocks. Then store the transforms of the cursor into blocks as well. Then using alignment and padding combine the two in the correct position

			// cursor starts at first option and needs to be handled by the cursor component
			auto& c_cursor = *gamestate->get_child(textbox_name_)->get_component<Component::Cursor>("cursor");

			std::vector<Component::Transform*> vec_cursor_transforms;
			auto c_cursor_src = e_spritesheet->get_component<Component::Src>(cursor_json_["id"].get<std::string>());

			for (auto cursor_block : cursor_blocks)
			{
				vec_cursor_transforms.push_back(cursor_block.transform);
				cursor_block.transform = c_cursor_src;
				cursor_block.src = c_cursor_src;
			}

			c_cursor.add_options(vec_cursor_transforms);

			auto e_glyphs = e_font->get_child("glyphs");
			auto blocks_i = 0;
			glm::vec2 current_pos(0);
			auto prev_char = 0;
			auto line_h = e_font->get_component<Component::Integer>("line_h")->value;

			auto space = line_h / 3.0f;

			std::vector<Component::Transform*> vec_options_transforms;

			for (auto option : options_)
			{
				for (auto curr_char : option)
				{
					if (curr_char == ' ')
					{
						current_pos.x += space; 
						continue;
					}

					auto& c_bitmap_char = *e_glyphs->get_component<Component::Rectangle::BitMapGlyph>(static_cast<std::size_t>(curr_char));

					auto& c_cur_char_src = *option_blocks[blocks_i].src;

					c_cur_char_src.x = c_bitmap_char.x;
					c_cur_char_src.y = c_bitmap_char.y;
					c_cur_char_src.w = c_bitmap_char.w;
					c_cur_char_src.h = c_bitmap_char.h;

					auto c_cur_char_transform = option_blocks[blocks_i++].transform;

					c_cur_char_transform->x = current_pos.x + c_bitmap_char.x_off + c_bitmap_char.check_kerning(prev_char);
					c_cur_char_transform->y = current_pos.y + c_bitmap_char.y_off;
					c_cur_char_transform->w = c_bitmap_char.w;
					c_cur_char_transform->h = c_bitmap_char.h;

					current_pos.x += c_bitmap_char.advance + c_bitmap_char.check_kerning(prev_char);

					prev_char = curr_char;

					vec_options_transforms.push_back(c_cur_char_transform);
				}
			}
			
			// can I shorten this? TODO
			if (cursor_json_["alignment"] == "right")
			{
				// TODO
			} 
			else if (cursor_json_["alignment"] == "left")
			{
				// TODO
			}
			else if (cursor_json_["alignment"] == "bottom")
			{
				// TODO
			}
			else if (cursor_json_["alignment"] == "top")
			{
				// TODO
			}
			else if (cursor_json_["alignment"] == "encapsulate")
			{
				// TODO
			}
			else
			{
				Logger::error("invalid cursor alignment", Logger::SEVERITY::HIGH);
			}
		}
	};
}