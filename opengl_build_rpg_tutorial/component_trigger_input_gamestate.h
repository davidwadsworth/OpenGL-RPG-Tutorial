#pragma once
#include "component_trigger_input.h"

constexpr auto MAX_TEX_UNITS = 32u;

namespace Component {
	namespace Trigger {
		namespace Input {
			class IGameState : public Component::Trigger::IInput
			{
				GLuint open_tex_unit_;
				virtual void _init() = 0;
			public:
				using Component::Trigger::IInput::IInput;

				void init() {
					open_tex_unit_ = 0u;
					_init();
				};
				virtual void destroy() = 0;
				virtual void run() = 0;

				GLuint get_open_tex_unit() 
				{
					if (open_tex_unit_ > MAX_TEX_UNITS)
						Logger::error("A lot of textures are being used, increment MAX_TEX_UNITS if this is normal", Logger::HIGH);

					return open_tex_unit_++; 
				}
			};
		}
	}

}
