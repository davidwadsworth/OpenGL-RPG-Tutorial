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
			class Texture : public Component::Trigger::IInput
			{
			public:
				Texture(std::string name)
					: Component::Trigger::IInput(name)
				{}

			private:
				void create(Entity* gamestate) override final
				{
					gamestate->get_child("observer")->add_id_component<Component::SystemObserver>("texture");

					auto flesh_tex_name = "resources/images/flesh_full.png";

					auto& c_flesh_tex = *entity_->add_id_component<Component::Texture>("flesh_full");
					c_flesh_tex.load(flesh_tex_name);
				}
			};
		}
		
	}
}
