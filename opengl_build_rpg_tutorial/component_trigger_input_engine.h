#pragma once
#include "component_vector.h"
#include "component_trigger_input.h"
#include "component_system.h"

/*
Holds our render and update queues for our game engine

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class Engine : public Component::Trigger::In
			{
			public:
				Engine(std::string name)
					: Component::Trigger::In(name)
				{}

			private:
				void create(EntityMap& map) override final
				{
					entity_->add_component_str_id<Component::SystemVector>("update");
					entity_->add_component_str_id<Component::SystemVector>("render");
				}
			};
		}
		
	}
}

