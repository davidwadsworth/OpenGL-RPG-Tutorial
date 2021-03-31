#pragma once
#include "component_system.h"
#include "game.h"

namespace ComponentSystem
{
	class DataIn : public Component::System
	{
	protected:
		Entity* entity_;
	public:

		DataIn(EntityMap& map, std::string name)
			: entity_(new Entity())
		{
			map.emplace(name, std::unique_ptr<Entity>(entity_));
		}

		void execute() override = 0;
	};
}

