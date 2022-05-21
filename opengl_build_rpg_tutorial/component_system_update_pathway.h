#pragma once
#include "component_navigator.h"
#include "component_system.h"
#include "component_trigger.h"
#include "tree.h"

namespace Component {
	namespace System {
		namespace Update
		{
			class Pathway : public Component::ISystem, Tre<Component::ITrigger> 
			{
				Component::Navigator& nav_;
			public:
				void execute() override
				{

				}
			};
		}
	}
}