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
				void create(Entity* gamestate) override final
				{
					auto flesh_tex_name = "resources/images/flesh_full.png";

					auto& c_flesh_tex = *entity_->add_component_str_id<Component::Texture>("flesh_full");
					c_flesh_tex.load(flesh_tex_name);
				}
			};
		}
		
	}
}