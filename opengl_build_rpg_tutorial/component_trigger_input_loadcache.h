#pragma once
#include "component_trigger_input.h"
#include "component_parseaction.h"

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class LoadCache : public Component::Trigger::IInput
			{
				void create(Entity* gamestate) override
				{
					auto& c_triggervec = *gamestate->get_component<Component::TriggerVector>("trigger");
					auto e_index = gamestate->get_child("index");
					entity_->add_id_component<Component::ParseAction>("parse", entity_, e_index, c_triggervec);
				}
			};
		}
	}
}
