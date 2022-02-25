#pragma once
#include "component_trigger_input.h"
#include "component_position.h"
#include "rect.h"
#include <glm/vec2.hpp>

/*
Creates our local camera using width and height

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input 
		{
			class Camera : public Component::Trigger::IInput
			{
			private:
				void create(Entity* gamestate) override final
				{
					entity_->add_component<Component::Position>(0.0f,0.0f);
				}

			};
		}
	}
}