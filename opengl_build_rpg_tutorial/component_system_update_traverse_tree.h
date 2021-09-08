#pragma once
#include "component_system.h"
#include "component_tree.h"
#include "component_trigger.h"

namespace Component {
	namespace System {
		namespace Update
		{
			class TraverseTree : public Component::ISystem
			{
				Component::Tree<std::vector<Component::ITrigger*>> trigger_tree_;

			public:

				void execute() override
				{

				}
			};
		}
	}
}
