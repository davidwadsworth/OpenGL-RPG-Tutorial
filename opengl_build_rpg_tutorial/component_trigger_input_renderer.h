#pragma once
#include "component_trigger_input.h"
#include "component_renderer.h"

/*
Basic batch renderer to draw multiple rectangular images.

@author David Wadsworth
*/


constexpr GLuint MAX_SPRITES = 255u;

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class Renderer : public Component::Trigger::IInput
			{

			public:
				Renderer(std::string name)
					: Component::Trigger::IInput(name)
				{}

			private:
				void create(Entity* gamestate) override final
				{
					auto& cti_observer = *gamestate->get_component<Component::Trigger::Input::SystemObs>(0);
					cti_observer.add_subscriber(name_);

					// create a renderer object and input appropriate attribute sizes and max amount of sprites on screen at once
					// 2 = pos, 2 = coords
					entity_->add_component<Component::Renderer>(std::vector<GLuint>{ 2, 2 }, MAX_SPRITES);
				}
			};
		}
		
	}
}
