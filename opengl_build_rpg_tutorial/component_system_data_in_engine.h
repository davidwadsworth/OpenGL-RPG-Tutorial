#pragma once
#include "component_vector.h"
#include "component_system_data_in.h"

namespace ComponentSystemDataIn
{
	class Engine : public ComponentSystem::DataIn
	{

	public:
		Engine(EntityMap& map, std::string name)
			: ComponentSystem::DataIn(map, name)
		{}

		void execute() override
		{
			entity_->push_back_component<Component::Vector<Component::System*>>();
			entity_->push_back_component<Component::Vector<Component::System*>>();
		}
	};
}

