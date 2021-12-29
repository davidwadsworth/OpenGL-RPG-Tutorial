#pragma once
#include "component_system.h"
#include "component_tree.h"
#include "component_trigger.h"
#include "component_controller.h"
#include "component_vector.h"

namespace Component {
	namespace System {
		namespace Update
		{
			class TraverseTree : public Component::ISystem
			{
				Component::IController& controller_;
				Tre<Component::ITrigger>::Node* cursor_;
				Component::TriggerVector& triggers_;
			public:
				TraverseTree(Component::TriggerTree& trigger_tree, Component::IController& controller, Component::TriggerVector& triggers)
					: controller_(controller), cursor_(trigger_tree.get_root()), triggers_(triggers)
				{}

				void execute() override
				{
					if (controller_.key_press_action_1())
						triggers_.push_back(cursor_->value);

				}
			};
		}
	}
}
