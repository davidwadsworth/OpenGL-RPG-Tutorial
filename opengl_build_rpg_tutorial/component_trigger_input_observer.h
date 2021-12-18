#pragma once
#include "component_vector.h"
#include "component_trigger_input.h"
#include "component_system.h"
#include "component_doubly_linked_list.h"
#include "component_observer.h"
#include <cstdarg>

/*
@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
		{
			template <typename T>
			class Obs : public Component::Trigger::IInput
			{
			public:
				Obs(std::string name)
					: Component::Trigger::IInput(name)
				{}

				void add_observed(std::vector<typename Component::DLL<T*>::Node*> nodes, std::vector<std::string> strings)
				{
					for (auto str : strings)
					{
						auto observed_objects = entity_->get_component<Component::Observer<T*>>(str)->observed_objects;
						observed_objects.insert(observed_objects.end(), nodes.begin(), nodes.end());
					}
				}

				void add_observed(typename Component::DLL<T*>::Node * node, std::vector<std::string> strings)
				{
					add_observed({ node }, strings);
				}

				Component::DLL<T*>& add_subscriber(std::string name)
				{
					return *entity_->add_id_component<Component::DLL<T*>>(name);
				}

			private:
				void create(Entity* gamestate) override final
				{}
			};
#define SystemObs Obs<ISystem>
		}

	}
}

