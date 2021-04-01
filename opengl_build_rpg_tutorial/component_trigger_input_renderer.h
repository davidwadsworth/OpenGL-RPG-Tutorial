#pragma once
#include "component_trigger_input.h"
#include "component_renderer.h"

constexpr GLuint MAX_SPRITES = 255u;

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class Renderer : public Component::Trigger::In
			{

			public:
				Renderer(std::string name)
					: Component::Trigger::In(name)
				{}

			private:
				void create(EntityMap& map) override final
				{
					// create a renderer object and input appropriate attribute sizes and max amount of sprites on screen at once
					// 2 = pos, 2 = coords
					entity_->add_component<Component::Renderer>(std::vector<GLuint>{ 2, 2 }, MAX_SPRITES);
				}
			};
		}
		
	}
}
