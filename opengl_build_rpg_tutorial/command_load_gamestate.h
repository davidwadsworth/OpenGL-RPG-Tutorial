#pragma once
#include "component_renderer.h"
#include "command_load.h"
#include "component_engine.h"
#include "component_texunit.h"
#include "component_command_pathway.h"

/*
Set up class for all game object creation within the overworld state

@author David Wadsworth
*/
namespace Command {
	namespace Load
	{
		class Gamestate : public Command::ILoad
		{
			std::string name_;
			nlohmann::json index_json_;
		public:

			void load(nlohmann::json json) override final
			{
				name_ = json["load"]["name"];
				index_json_ = json["load"];
			}

			void execute(Entity* gamestate) override final
			{
				auto e_gamestate = gamestate->add_id_child(name_);

				// set up renderer and engine systems for run
				e_gamestate->add_id_component<Component::Engine>("render");
				e_gamestate->add_id_component<Component::Engine>("update");
				auto pathway = e_gamestate->add_id_component<Component::CCommand::Pathway>("pathway");

				// load in game objects
				Game::global->get_component<Component::CCommand::Pathway>("pathway")->add_next(index_json_);
			}
		};
	}
}
