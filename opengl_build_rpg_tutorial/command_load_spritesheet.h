#pragma once
#include "component_trigger_input.h"
#include "component_json.h"
#include "delimiter_split.h"
#include "component_material.h"
#include "component_texunit.h"
#include "component_rect.h"

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class SpriteSheet : public Component::Trigger::IInput
			{
				void create(Entity* gamestate)
				{
					auto& spritesheet_json = gamestate->get_child("index")->get_component<Component::Json>(name_)->json;

					nlohmann::json frames = spritesheet_json["frames"];

					for (auto& frame : frames)
					{
						std::string filename = frame["filename"];
						auto name = delimiter_split(filename.c_str(), '.')[0];

						float src_x = frame["frame"]["x"];
						float src_y = frame["frame"]["y"];
						float src_w = frame["frame"]["w"];
						float src_h = frame["frame"]["h"];
						auto c_src = entity_->add_id_component<Component::Src>(name, Rect(src_x, src_y, src_w, src_h));

					}

					std::string image_path = spritesheet_json["meta"]["image"];
					auto image_name = delimiter_split(image_path.c_str(), '.')[0];

					auto& c_ss_texture = *gamestate->get_child("texture")->get_component<Component::Texture>(image_name);
					auto& c_sprite_shader = *gamestate->get_child("shader")->get_component<Component::Shader>("sprite");

					auto& c_texunit = *gamestate->get_component<Component::TexUnit>("texunit");

					entity_->add_id_component<Component::Material>("material", c_ss_texture, c_sprite_shader, c_texunit.get_open_tex_unit());
				}
			public:
				using Component::Trigger::IInput::IInput;
			};
		}
	}
}