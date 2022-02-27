#pragma once
#include "component_trigger_input.h"
#include "component_trigger_input_camera.h"
#include "component_trigger_input_controller.h"
#include "component_trigger_input_shader.h"
#include "component_trigger_input_texture.h"
#include "component_trigger_input_gameobj_tilemap.h"
#include "component_trigger_input_gameobj_player.h"
#include "component_trigger_input_qt.h"
#include "component_trigger_input_gameobj_collidermap.h"
#include "component_trigger_input_font.h"
#include "component_trigger_input_gameobj_text.h"
#include "component_trigger_input_gamestate.h"
#include "component_engine.h"
#include "component_trigger_input_gameobj_textbox.h"

/*
Set up class for all game object creation within the overworld state

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace GameState
			{
				class Overworld : public Component::Trigger::Input::IGameState
				{
					std::vector<Component::ISystem*> update_;
					std::vector<Component::ISystem*> render_;
					Component::TriggerVector* c_triggers_ = nullptr;
					Component::Renderer* c_renderer_ = nullptr;

				public:
					using Component::Trigger::Input::IGameState::IGameState;

					void init() override final
					{
						auto overworld_objs = entity_->get_component_list();

						// init game objs
						for (auto obj : overworld_objs)
							static_cast<Component::ITrigger*>(obj)->execute(entity_);

						c_renderer_->init();
					}


					void destroy() override final
					{
						c_renderer_->release();
						// destroy all active game objs
						entity_->clear_children();
						update_.clear();
						render_.clear();
					}

					void run() override final
					{
						// make updates to live entities
						for (auto u : update_)
							u->execute();

						c_renderer_->begin();

						// make draw calls to renderer
						for (auto r : render_)
							r->execute();

						c_renderer_->end();

						for (auto t : *c_triggers_)
							t->execute(entity_);
						c_triggers_->clear();
					}

				private:
					void create(Entity* gamestate) override final
					{
						// set up renderer and engine systems for run
						auto game_engine = new Entity();
						entity_->add_id_child(game_engine, "engine");

						auto& c_render_engine = *game_engine->add_id_component<Component::Engine>("render");
						auto& c_update_engine = *game_engine->add_id_component<Component::Engine>("update");
						c_triggers_ = game_engine->add_id_component<Component::TriggerVector>("trigger");
						c_renderer_ = game_engine->add_id_component<Component::Renderer>("renderer", std::vector<GLuint>{2u, 2u}, 255u);

						entity_->add_id_ct_input<Component::Trigger::Input::Camera>("camera");
						entity_->add_id_ct_input<Component::Trigger::Input::Controller>("controller");
						entity_->add_id_ct_input<Component::Trigger::Input::Shader>("shader_manager");
						entity_->add_id_ct_input<Component::Trigger::Input::Texture>("texture_manager");
						entity_->add_id_ct_input<Component::Trigger::Input::Font>("gilsans");
						entity_->add_id_ct_input<Component::Trigger::Input::GJKQuadTree>("collision_world");
						entity_->add_id_ct_input<Component::Trigger::Input::GameObj::TileMap>("tilemap");
						entity_->add_id_ct_input<Component::Trigger::Input::GameObj::Player>("player");
						entity_->add_id_ct_input<Component::Trigger::Input::GameObj::ColliderMap>("collider_map");
						entity_->add_id_ct_input<Component::Trigger::Input::GameObj::TextBox>("textbox");

						update_ = c_update_engine.flatten();
						render_ = c_render_engine.flatten();
					}
				};
			}
		}
	}
}
