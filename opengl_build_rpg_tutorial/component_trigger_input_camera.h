#pragma once
#include "load.h"
#include "component_position.h"

/*
Creates our local camera using width and height

@author David Wadsworth
*/
namespace Command {
	namespace Load {
		class Camera : public ILoad
		{
			std::string name;
		public:

			void load(nlohmann::json json) override final
			{
				name = json["name"];
			}

			void execute(Entity* gamestate) override final
			{
				gamestate->add_id_child(name)->add_component<Component::Position>(0.0f, 0.0f);
			}
		};
	}
}