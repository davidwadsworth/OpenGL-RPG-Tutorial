#pragma once
#include "component_system.h"
#include "component_tree.h"
#include "component_trigger.h"
#include "component_controller.h"

namespace Component {
	namespace System {
		namespace Update
		{
			class TraverseTree : public Component::ISystem
			{
				Component::TriggerTree& trigger_tree_;
				Component::IController& controller_;
				Tre<Component::ITrigger>::Node* cursor_;
			public:
				TraverseTree(Component::TriggerTree& trigger_tree, Component::IController controller)
					: trigger_tree_(trigger_tree), controller_(controller), cursor_(trigger_tree.get_root())
				{}


				void execute() override
				{

				}
			};
		}
	}
}
