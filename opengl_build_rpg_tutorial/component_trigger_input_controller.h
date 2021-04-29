#pragma once
#include "component_trigger_input.h"
#include "component_controller_keyboard.h"

/*
Sets up our keyboard controller for our game.

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input 
		{
			class Controller : public Component::Trigger::In
			{
			public:
				Controller(std::string name)
					: Component::Trigger::In(name)
				{}

			private:
				void create(Entity* gamestate) override final
				{
					entity_->add_component<Component::Controller::Keyboard>();
				}
			};
		}
	}
}

