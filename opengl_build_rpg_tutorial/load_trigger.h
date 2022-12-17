#pragma once
#include "load.h"
#include "component_trigger.h"

namespace Load
{
	class Trigger : public ILoad
	{
		std::string textbox_, trigger_;
	public:
		void load(nlohmann::json json)
		{
			trigger_ = json["load"]["data"];
			textbox_ = json["textbox"]["name"];
		}

		void execute(Entity* gamestate) override
		{
			gamestate->get_child(textbox_)->get_component<Component::ITrigger>(trigger_)->execute(gamestate);
		}
	};
}
