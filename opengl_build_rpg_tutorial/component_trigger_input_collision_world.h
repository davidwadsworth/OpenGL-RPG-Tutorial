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
					gamestate->get_child("observer")->add_id_component<Component::SystemObserver>("collision world");
					this->entity_->add_component<Component::GJKVector>();
				}
			};
		}
	}
}
