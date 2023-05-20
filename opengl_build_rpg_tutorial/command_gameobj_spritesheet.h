#pragma once
#include "component_json.h"
#include "delimiter_split.h"
#include "component_material.h"
#include "component_texunit.h"
#include "component_rect.h"
#include "command.h"

namespace Command {
	namespace GameObj {
		class SpriteSheet : public ICommand
		{
			std::string name_;
			std::string image_name_;
			nlohmann::json frames_;
		public:
			void load(nlohmann::json json) override
			{
				name_ = json["name"];
				std::string image_path = json["meta"]["image"];
				image_name_ = delimiter_split(image_path.c_str(), '.')[0];
				frames_ = json["frames"];
			}

			void execute(Entity* gamestate) override
			{
				auto e_spritesheet = gamestate->add_id_child(name_);

				for (auto& frame : frames_)
				{
					std::string filename = frame["filename"];
					auto name = delimiter_split(filename.c_str(), '.')[0];

					float src_x = frame["frame"]["x"];
					float src_y = frame["frame"]["y"];
					float src_w = frame["frame"]["w"];
					float src_h = frame["frame"]["h"];
					auto c_src = e_spritesheet->add_id_component<Component::Src>(name, Rect(src_x, src_y, src_w, src_h));
				}

				auto& c_ss_texture = *gamestate->get_child("texture")->get_component<Component::Texture>(image_name_);
				auto& c_sprite_shader = *gamestate->get_child("shader")->get_component<Component::Shader>("sprite");

				auto& c_texunit = *gamestate->get_component<Component::TexUnit>("texunit");

				e_spritesheet->add_id_component<Component::Material>("material", c_ss_texture, c_sprite_shader, c_texunit.get_open_tex_unit());
			}
		};
	}
}