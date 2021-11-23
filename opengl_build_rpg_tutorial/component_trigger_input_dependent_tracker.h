#pragma once
#include "component_trigger_input.h"
#include "component_transform.h"
#include "rect.h"
#include <glm/vec2.hpp>
#include "component_observer.h"

/*
@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class DependentObserver : public Component::Trigger::IInput
			{
			public:
				DependentObserver(std::string name)
					: Component::Trigger::IInput(name)
				{}
			private:
				void create(Entity* gamestate) override final
				{
					entity_->add_component<Component::SystemObserver>();
				}
			};
		}
	}
}