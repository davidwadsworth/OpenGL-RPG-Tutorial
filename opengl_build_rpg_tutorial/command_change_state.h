#pragma once
#include "command.h"
#include "game.h"
#include "component_state.h"
#include "component_engine.h"

namespace Command
{
	class ChangeState : public ICommand
	{
		nlohmann::json new_state;
		
		nlohmann::json json_to_system(Entity* e_gamestate, nlohmann::json json)
		{
			
		}

	
	public:

		virtual void load(nlohmann::json json) 
		{
			new_state = json["load"];
		}


		virtual void execute(Entity* e_gamestate)
		{
			auto& c_state = *Game::global->get_component<Component::State>("state");
			
			auto current = json_to_system(e_gamestate, new_state);
			c_state.previous_state = c_state.current_state;
			c_state.current_state = current;
		}
	};
}



