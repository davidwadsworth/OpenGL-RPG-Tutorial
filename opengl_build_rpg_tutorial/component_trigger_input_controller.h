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
				Controller(std::string name)
					: Component::Trigger::IInput(name)
				{}

			private:
				void create(Entity* gamestate) override final
				{
					gamestate->get_child("observer")->add_id_component<Component::SystemObserver>("controller");
					entity_->add_component<Component::Controller::Keyboard>();
				}
			};
		}
	}
}

