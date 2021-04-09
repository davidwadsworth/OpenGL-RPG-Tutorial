#pragma once
#include "component_trigger_input.h"
#include "component_texture.h"

/*
Loads and stores Textures for overworld

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class Texture : public Component::Trigger::In
			{
			public:
				Texture(std::string name)
					: Component::Trigger::In(name)
				{}

			private:
				void create(EntityMap& map) override final
				{
					auto flesh_tex_name = "resources/images/flesh_full.png";
					auto tileset_tex_name = "resources/images/iq_shape_padding.png";
					auto tileset_nsp_tex_name = "resources/images/iq_no_shape_padding.png";

					auto& c_flesh_tex = *entity_->add_component_str_id<Component::Texture>("flesh_full");
					c_flesh_tex.load(flesh_tex_name);

					auto& c_tileset_tex = *entity_->add_component_str_id<Component::Texture>("iq_shape_padding");
					c_tileset_tex.load(tileset_tex_name);

					auto& c_tileset_nsp_tex = *entity_->add_component_str_id<Component::Texture>("iq_no_shape_padding");
					c_tileset_tex.load(tileset_nsp_tex_name);
				}
			};
		}
		
	}
}
