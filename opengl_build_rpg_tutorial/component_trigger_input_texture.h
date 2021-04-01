#pragma once
#include "component_trigger_input.h"
#include "component_texture.h"

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
					auto grass_tex_name = "resources/images/grass.png";

					auto& c_flesh_tex = *entity_->add_component_str_id<Component::Texture>("flesh");
					c_flesh_tex.load(flesh_tex_name);

					auto& c_grass_tex = *entity_->add_component_str_id<Component::Texture>("grass");
					c_grass_tex.load(grass_tex_name);
				}
			};
		}
		
	}
}
