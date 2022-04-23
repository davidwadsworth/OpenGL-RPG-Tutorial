#pragma once
#include "component_trigger_input.h"
#include "component_trigger_load_switchstate.h"

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class LoadCache : public Component::Trigger::IInput
			{
				void create(Entity* gamestate) override
				{
					entity_->add_id_component<Component::Trigger::Load::SwitchState>("switch_state");
				}
			public:
				using Component::Trigger::IInput::IInput;
			};
		}
	}
}
