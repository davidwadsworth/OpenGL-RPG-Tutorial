#pragma once
#include "component_trigger_input.h"
#include "component_vector.h"
#include "component_collider.h"

/*
List of all active colliding objects in game world.

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class CollisionWorld : public Component::Trigger::IInput
			{
			public:
				CollisionWorld(std::string name)
					: Component::Trigger::IInput(name)
				{}

			private:
				void create(Entity* gamestate) override
				{
					auto& cti_observer = *gamestate->get_component<Component::Trigger::Input::SystemObs>(0);
					cti_observer.add_subscriber(name_);

					this->entity_->add_component<Component::GJKVector>();
				}
			};
		}
	}
}
