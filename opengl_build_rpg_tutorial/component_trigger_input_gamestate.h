#pragma once
#include "component_trigger_input.h"
#include "component_texunit.h"

namespace Component {
	namespace Trigger {
		namespace Input {
			class IGameState : public Component::Trigger::IInput
			{
				virtual void _init() = 0;
			public:
				using Component::Trigger::IInput::IInput;

				void init() {
					Game::global->get_component<Component::TexUnit>("texunit")->reset();
					_init();
				};
				virtual void destroy() = 0;
				virtual void run() = 0;

			};
		}
	}

}
