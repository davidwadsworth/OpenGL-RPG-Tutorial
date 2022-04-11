#pragma once
#include "component_trigger_input.h"

namespace Component {
	namespace Trigger {
		namespace Input {
			class IGameState : public Component::Trigger::IInput
			{
			public:
				using Component::Trigger::IInput::IInput;

				virtual void init() = 0;
				virtual void destroy() = 0;
				virtual void run() = 0;
			};
		}
	}

}
