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
			class CollisionWorld : public Component::Trigger::In
			{
			public:
				CollisionWorld(std::string name)
					: Component::Trigger::In(name)
				{}

			private:
				void create(EntityMap& map) override
				{
					this->entity_->add_component<Component::ColliderVector>();
				}
			};
		}
	}
}
