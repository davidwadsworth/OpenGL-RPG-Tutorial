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
				void create(Entity* gamestate) override final
				{
					entity_->add_id_component<Component::SystemVector>("update");
					entity_->add_id_component<Component::SystemVector>("render");
				}
			};
		}
		
	}
}

