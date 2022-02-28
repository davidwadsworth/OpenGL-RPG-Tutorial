#pragma once
#include "component_trigger.h"
#include <vector>

namespace Component {
	namespace Trigger
	{
		class SwapVectors : public Component::ITrigger
		{
			std::vector<ISystem*>& v1_;
		public:
			SwapVectors(std::vector<ISystem*>& v1)
				: v1_(v1)
			{}

			void execute(Entity* gamestate) override
			{
				auto& c_update = *gamestate->get_component<Component::SystemVector>("update_vec");
				c_update.swap(v1_);
			}
		};
	}
}