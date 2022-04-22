#pragma once
#pragma once
#include "component_trigger_load.h"
#include "component_rect.h"

namespace Component {
	namespace Trigger {
		namespace Load
		{
			class SwitchState : public Component::Trigger::ILoad
			{
				glm::vec2 return_pos;
			public:

				void load(nlohmann::json json)
				{
					float x = json["return_pos"]["x"];
					float y = json["return_pos"]["y"];
					return_pos = glm::vec2(x, y);
				}

				void execute(Entity* gamestate) override
				{


				}
			};
		}
	}
}