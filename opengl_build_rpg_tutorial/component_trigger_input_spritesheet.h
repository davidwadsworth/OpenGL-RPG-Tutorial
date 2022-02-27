#pragma once
#include "component_trigger_input.h"
#include "component_json.h"
#include "delimiter_split.h"
#include "component_material.h"

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

					}

					std::string image_path = spritesheet_json["meta"]["image"];
					auto image_name = delimiter_split(image_path.c_str(), '.')[0];

					auto& c_ss_texture = *gamestate->get_child("texture")->get_component<Component::Texture>(image_name);
					auto& c_sprite_shader = *gamestate->get_child("shader")->get_component<Component::Shader>("sprite");

					entity_->add_component<Component::Material>(c_ss_texture, c_sprite_shader, 2);
				}
			public:

			};
		}
	}
}