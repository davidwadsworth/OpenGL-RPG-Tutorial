#pragma once
#include "component_vector.h"
#include "component_trigger_input.h"
#include "component_system.h"
#include "component_grouped_objects.h"
#include "component_trigger_clean_engine.h"

/*
Holds our render and update queues for our game engine

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class Engine : public Component::Trigger::IInput
			{
				std::size_t group_sizes_;
			public:
				Engine(std::string name, std::size_t group_sizes)
					: Component::Trigger::IInput(name), group_sizes_(group_sizes)
				{}

			private:
				void create(Entity* gamestate) override final
				{
					entity_->add_id_component<Component::SystemVector>("update");
					entity_->add_id_component<Component::SystemVector>("render");
					entity_->add_id_component<Component::TriggerVector>("trigger");
					entity_->add_component<Component::Trigger::CleanEngine>();
				}
			};
		}
		
	}
}

