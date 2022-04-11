#pragma once
#include "component_trigger_input.h"
#include "component_controller_keyboard.h"
#include "component_json.h"

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
					auto controller_json = Game::global->get_child("index")->get_component<Component::Json>(name_)->json;

					entity_->add_component<Component::Controller::Keyboard>();
				}
			};
		}
	}
}

