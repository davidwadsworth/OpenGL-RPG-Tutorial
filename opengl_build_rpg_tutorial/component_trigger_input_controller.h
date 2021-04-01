#pragma once
#include "component_trigger_input.h"
#include "component_controller_keyboard.h"

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
				void create(EntityMap& map) override final
				{
					entity_->add_component<Component::Controller::Keyboard>();
				}
			};
		}
	}
}

