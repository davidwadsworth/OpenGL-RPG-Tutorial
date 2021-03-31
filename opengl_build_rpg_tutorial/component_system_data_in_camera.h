#pragma once
#include "component_system_data_in.h"
#include "component_transform.h"
#include "rect.h"

namespace ComponentSystemDataIn
{
	class Camera : public ComponentSystem::DataIn
	{
		Rect boundaries_;
	public:

		Camera(EntityMap& map, std::string name, Rect boundaries)
			: ComponentSystem::DataIn(map, name), boundaries_(boundaries)
		{}

		void execute() override
		{
			// set up camera boundaries
			entity_->add_component<Component::Transform>(boundaries_);
		}

	};
}