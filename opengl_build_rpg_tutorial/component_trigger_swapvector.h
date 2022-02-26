#pragma once
#include "component_trigger.h"

namespace Component {
	namespace Trigger
	{
		class SwapVector : public Component::ITrigger
		{
			std::vector& v1_, & v2_;
		public:
			SwapVector(std::vector& v1, std::vector& v2)
				: v1_(v1), v2_(v2)
			{}

			void execute(Entity* gamestate) override
			{
				v1_.swap(v2_);
			}
		};
	}
}