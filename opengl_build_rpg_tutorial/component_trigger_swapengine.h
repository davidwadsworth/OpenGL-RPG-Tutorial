#pragma once
#pragma once
#include "component_trigger.h"
#include "component_engine.h"

namespace Component {
	namespace Trigger
	{
		class SwapEngine : public Component::ITrigger
		{
			Component::Engine& e1_, &e2_;
		public:
			SwapEngine(Component::Engine& e1, Component::Engine& e2)
				: e1_(e1), e2_(e2)
			{}

			void execute(Entity* gamestate) override
			{
				e1_.swap(e2_);
			}
		};
	}
}