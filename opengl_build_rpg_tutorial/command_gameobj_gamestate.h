#pragma once
#include "component_renderer.h"
#include "command.h"
#include "component_engine.h"
#include "component_texunit.h"
#include "component_pathway.h"
#include "game.h"

/*
Set up class for all game object creation within the overworld state

@author David Wadsworth
*/
namespace Command {
	namespace GameObj
	{
		class Gamestate : public ICommand
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
				auto pathway = e_gamestate->add_id_component<Component::Pathway>("pathway");

				// load in game objects
				pathway->add_next(index_json_); // TODO check if this is right
			}
		};
	}
}
