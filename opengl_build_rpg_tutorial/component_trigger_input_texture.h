#pragma once
#include "component_trigger_input.h"
#include "component_texture.h"
#include "component_json.h"
#include "delimiter_split.h"

/*
Loads and stores Textures for overworld

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class Texture : public Component::Trigger::IInput
			{
			public:
				using Component::Trigger::IInput::IInput;
			private:
				void create(Entity* gamestate) override final
				{
					auto& texture_json = gamestate->get_child("index")->get_component<Component::Json>(name_)->json;
					
					for (std::string tex_file : texture_json)
					{
						auto tex_name = delimiter_split(delimiter_split(tex_file.c_str(), '/').back().c_str(), '.')[0];
						auto& c_texture = *entity_->add_id_component<Component::Texture>(tex_name);
						tex_file = "Resources/Images/" + tex_file;
						c_texture.load(tex_file.c_str());
					}
				}
			};
		}
		
	}
}
