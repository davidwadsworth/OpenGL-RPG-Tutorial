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
					auto& cti_observer = *gamestate->get_component<Component::Trigger::Input::SystemObs>(0);
					cti_observer.add_subscriber(name_);

					entity_->add_component<Component::Controller::Keyboard>();
				}
			};
		}
	}
}

