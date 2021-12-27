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
			class Controller : public Component::Trigger::IInput
			{
			public:
				using Component::Trigger::IInput::IInput;

			private:
				void create(Entity* gamestate) override final
				{
					entity_->add_component<Component::Controller::Keyboard>();
				}
			};
		}
	}
}

