#pragma once
#include "load.h"
#include "component_system_render_blockdraw.h"
#include "component_cursor.h"
#include "component_template.h"
#include "component_rect_bitmap_glyph.h"

namespace Load
{
	class OptionBox : public ILoad
	{
		std::string font_name_, align_h_, align_v_, textbox_name_, nav_;
		float font_sc_, render_group_, option_pos_x_, option_pos_y_, outline_;
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
			float padding_x = json["textbox"]["textarea"]["msg_padding"]["x"];
			float padding_y = json["textbox"]["textarea"]["msg_padding"]["y"];

			float pos_x = json["load"]["transform"]["x"];
			float pos_y = json["load"]["transform"]["y"];

			outline_ = json["textbox"]["optionbox"]["outline"];

			option_pos_x_ = json["textbox"]["optionbox"]["pos_x"];
			option_pos_y_ = json["textbox"]["optionbox"]["pos_y"];

			option_pos_x_ = option_pos_x_ * box_w + pos_x;
			option_pos_y_ = option_pos_y_ * box_h + pos_y;

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
			
			auto char_count = std::accumulate(options_.begin(), options_.end(), 0, [](std::size_t total, const std::string& option)
				{
					return total + option.size();
				});

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
			auto line_i = 0;
			auto cursor_i = 0;
			glm::vec2 current_pos(option_pos_x_, option_pos_y_);
			auto prev_char = 0;
			auto line_h = e_font->get_component<Component::Integer>("line_h")->value;

			auto space = line_h / 3.0f;
			
			std::vector<std::vector<Component::Transform*>> vec_options_transforms;
			vec_options_transforms.push_back(std::vector<Component::Transform*>());
			std::vector<glm::vec3> option_lengths;
			
			for (auto option : options_)
			{
				auto x = current_pos.x;
				auto y = current_pos.y;
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

					vec_options_transforms[line_i].push_back(c_cur_char_transform);
				}

				option_lengths.push_back(glm::vec3{ x, y, vec_options_transforms[line_i].back()->x + vec_options_transforms[line_i].back()->w - vec_options_transforms[line_i].front()->x });
				vec_options_transforms[line_i].push_back(vec_cursor_transforms[cursor_i++]);

				if (nav_ == "optionboxh")
				{
					current_pos.y += line_h + outline_ + padding_;
					vec_options_transforms.push_back(std::vector<Component::Transform*>());
					line_i++;
				}
				else
					current_pos.x += line_h;
			}
			


			auto text_length = 0.0f;
			auto text_height = 0.0f;
			auto max_length = line_h - padding_ * 2.0f;
			auto cursor_scale = std::min(max_length / c_cursor_src->w, max_length / c_cursor_src->h);
			auto cursor_w = c_cursor_src->w * cursor_scale;
			auto cursor_h = c_cursor_src->h * cursor_scale;

			auto i = 0;
			for (auto cursor : cursor_blocks)
			{
				auto option_dimension = option_lengths[i];
				if (cursor_json_["alignment"] == "right")
				{
					cursor.transform->x = cursor_w < max_length ? option_dimension.x + option_dimension.z + padding_ + (max_length - cursor_w) / 2.0f
						: option_dimension.x + option_dimension.z + padding_;
					cursor.transform->y = cursor_h < max_length ? option_dimension.y : option_dimension.y + (max_length - cursor_h) / 2.0f;
					cursor.transform->w = cursor_w;
					cursor.transform->h = cursor_h;
				}
				else if (cursor_json_["alignment"] == "left")
				{
					cursor.transform->x = cursor_w < max_length ? option_dimension.x - padding_ - (max_length - cursor_w) / 2.0f
						: option_dimension.x - padding_;
					cursor.transform->y = cursor_h < max_length ? option_dimension.y : option_dimension.y + (max_length - cursor_h) / 2.0f;
					cursor.transform->w = cursor_w;
					cursor.transform->h = cursor_h;
				}
				else if (cursor_json_["alignment"] == "bottom")
				{
					cursor.transform->x = option_dimension.x - padding_;
					cursor.transform->y = cursor_h + line_h + padding_;
					cursor.transform->w = option_dimension.z;
					cursor.transform->h = outline_; 
				}
				else if (cursor_json_["alignment"] == "top")
				{
					cursor.transform->x = option_dimension.x;
					cursor.transform->y = option_dimension.y - padding_;
					cursor.transform->w = option_dimension.z;
					cursor.transform->h = outline_;
				}
				else if (cursor_json_["alignment"] == "encapsulate")
				{
					cursor.transform->x = option_dimension.x - padding_;
					cursor.transform->y = option_dimension.y - padding_;
					cursor.transform->w = option_dimension.z + padding_ * 2.0f;
					cursor.transform->h = line_h + padding_ * 2.0f;
				}
				else
				{
					Logger::error("invalid cursor alignment", Logger::SEVERITY::HIGH);
				}
			}
			
			// TODO
			// alignment for optionbox around pos x and y if text is out of bounds of the box after alignment then it 
			// is corrected by the box padding and line_h so that stay in bounds
			// need to line the text to a point and if any of the text lie out of bounds correct by box padding

			auto x_offset = std::vector<float>(vec_options_transforms.size(), 0.0f); // align left
			auto y_offset = 0.0f; // align top

			for (auto i = 0; i < vec_options_transforms.size(); ++i)
			{
				auto line_segment = option_pos_x_ - (((*(vec_options_transforms[i].end() - 1))->x + (*(vec_options_transforms[i].end() - 1))->w) - (*vec_options_transforms[i].begin())->x);

				if (align_h_ == "middle")
					x_offset[i] = line_segment / 2.0f;
				else if (align_h_ == "right")
					x_offset[i] = line_segment;
			}

			auto y_lowest = FLT_MAX;
			auto y_highest = -FLT_MAX;

			for (auto transform : *vec_options_transforms.begin())
				if (transform->y < y_lowest)
					y_lowest = transform->y;

			for (auto transform : *(vec_options_transforms.end() - 1))
				if (transform->y + transform->h > y_highest)
					y_highest = transform->y + transform->h;

			if (y_lowest == FLT_MAX)
				y_lowest = option_pos_y_;
			if (y_highest == -FLT_MAX)
				y_highest = option_pos_y_;

			if (align_v_ == "middle")
				y_offset = (option_pos_y_ - (y_highest - y_lowest)) / 2.0f;
			else if (align_v_ == "bottom")
				y_offset = (option_pos_y_ - (y_highest - y_lowest));

			std::vector < Component::Transform* > transform_vec;
			for (auto i = 0; i < vec_options_transforms.size(); ++i)
				for (auto transform : vec_options_transforms[i])
				{
					transform->x += x_offset[i];
					transform->y += y_offset;
					transform_vec.push_back(transform);
				}
		}
	};
}