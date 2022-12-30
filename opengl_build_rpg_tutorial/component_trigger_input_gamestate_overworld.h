#pragma once
#include "component_renderer.h"
#include "load.h"
#include "component_engine.h"
#include "component_texunit.h"
#include "component_system_update_pathway.h"

/*
Set up class for all game object creation within the overworld state

@author David Wadsworth
*/
namespace Component {
	namespace Trigger {
		namespace Input {
			namespace GameState
			{
				class Single2DV1 : public ILoad
				{
					Component::Engine* c_render_engine_, * c_update_engine_;
					nlohmann::json index_json_;
				public:
					std::string name, renderer;

					void load(nlohmann::json json) override final
					{
						name = json["name"];
						renderer = json["renderer"];
						index_json_ = json["load"];

					}

					void execute(Entity* gamestate) override final
					{
						auto e_gamestate = gamestate->add_id_child(name);

						// set up renderer and engine systems for run
						e_gamestate->add_id_component<Component::Engine>("render");
						e_gamestate->add_id_component<Component::Engine>("update");

						// load in game objects
						Game::global->get_component<Component::System::Update::Pathway>("pathway")->add_next(index_json_);
					}
				};
			}
		}
	}
}
