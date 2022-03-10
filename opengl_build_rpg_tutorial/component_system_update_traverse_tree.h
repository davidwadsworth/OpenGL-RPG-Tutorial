#pragma once
#include "component_system.h"
#include "component_tree.h"
#include "component_trigger.h"
#include "component_controller.h"
#include "component_vector.h"

/*
increments through the tree data structure using action_1 presses.

@author David Wadsworth
*/

namespace Component {
	namespace System {
		namespace Update
		{
			class TraverseTree : public Component::ISystem
			{
				Component::IController& controller_;
				Component::TriggerTree::Node* cursor_;
				Component::TriggerVector& triggers_;
			public:
				TraverseTree(Component::IController& controller, Component::TriggerVector& triggers)
					: controller_(controller), cursor_(nullptr), triggers_(triggers)
				{}

				void set_cursor(Component::TriggerTree& trigger_tree)
				{
					cursor_ = trigger_tree.get_head();
				}

				void execute() override
				{
					if (controller_.key_press_action_1())
					{
						if (!cursor_)
							Logger::error("Tree Traverse failed", Logger::HIGH);

						auto& cursor_item = cursor_->item;
						triggers_.insert(triggers_.end(), cursor_item.begin(), cursor_item.end());
						cursor_ = cursor_->next;
					}
						
				}
			};
		}
	}
}
