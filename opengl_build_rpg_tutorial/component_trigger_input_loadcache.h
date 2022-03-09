#pragma once
#include "component_trigger_input.h"

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class LoadCache : public Component::Trigger::IInput
			{
				void create(Entity* gamestate) override
				{}
			public:
				using Component::Trigger::IInput::IInput;
			};
		}
	}
}
