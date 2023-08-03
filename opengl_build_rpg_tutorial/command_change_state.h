#pragma once
#include "command.h"
#include "game.h"
#include "component_state.h"

namespace Command
{
	class ChangeState : public ICommand
	{
		nlohmann::json new_state;
	public:

		virtual void load(nlohmann::json json) 
		{
			new_state = json["load"];
		}
		virtual void execute(Entity* e_gamestate)
		{
			auto& c_state = *Game::global->get_component<Component::State>("state");
			
			for (auto r : new_state["render"])
				if (r.get<std::string>() == "current")


			
			c_state.previous_state = c_state.current_state;
		}
	};
}



