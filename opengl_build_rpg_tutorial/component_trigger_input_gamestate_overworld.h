#pragma once
#include "component_trigger_input.h"
#include "component_trigger_input_camera.h"
#include "component_trigger_input_controller.h"
#include "component_trigger_input_shader.h"
#include "component_trigger_input_texture.h"
#include "component_trigger_input_game_obj_tilemap.h"
#include "component_trigger_input_game_obj_player.h"
#include "component_trigger_input_collision_world.h"
#include "component_trigger_input_game_obj_collider_map.h"
#include "component_trigger_input_font.h"
#include "component_trigger_input_game_obj_text.h"
#include "component_trigger_input_gamestate.h"
#include "component_grouped_objects.h"
#include "component_system_clean_engine.h"

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
					Component::GroupedSystems* c_update_systems_ = nullptr;
					Component::GroupedSystems* c_render_systems_ = nullptr;
					Component::TriggerVector* c_triggers_ = nullptr;
					Component::TriggerVector* c_before_triggers_ = nullptr;
					Component::Renderer* c_renderer_ = nullptr;

				public:
					using Component::Trigger::Input::IGameState::IGameState;

					void init() override final
					{
						auto overworld_objs = entity_->get_component_list();

						// init information holding entities for active game objs
						for (auto obj : overworld_objs)
							static_cast<Component::Trigger::IInput*>(obj)->execute(entity_);

						auto e_game_objs = entity_->get_child("game_objs");
						auto overworld_game_objs = e_game_objs->get_component_list();

						// init game objs
						for (auto game_obj : overworld_game_objs)
						{
							auto cti_game_obj = static_cast<Component::Trigger::Input::IGameObj*>(game_obj);
							cti_game_obj->execute(e_game_objs);
							auto ct_add_game_obj = cti_game_obj->e_game_info_->get_component<Component::Trigger::AddGameObj>("add");
							c_triggers_->push_back(ct_add_game_obj);
						}

						c_renderer_->init();
					}


					void destroy() override final
					{
						// destroy all active game objs
						entity_->clear_children();

						// remove engine and renderer baggage
						c_render_systems_->clear();
						c_update_systems_->clear();
						c_triggers_->clear();
						c_renderer_->release();
					}

					void run() override final
					{
						for (auto bt : *c_before_triggers_)
							bt->execute(entity_);

						// make updates to live entities
						for (auto u : c_update_systems_->groups)
							for (auto i = 0u; i < u.size; ++i)
								u[i]->execute();

						c_renderer_->begin();

						// make draw calls to renderer
						for (auto r : c_render_systems_->groups)
							for (auto i = 0u; i < r.size; ++i)
								r[i]->execute();

						c_renderer_->end();

						for (auto t : *c_triggers_)
							t->execute(entity_);

					}

				private:
					void create(Entity* gamestate) override final
					{
						// set up renderer and engine systems for run
						auto c_render_systems_ = entity_->add_id_component<Component::GroupedSystems>("render");
						auto c_update_systems_ = entity_->add_id_component<Component::GroupedSystems>("update");
						auto c_triggers_ = entity_->add_id_component<Component::TriggerVector>("trigger");
						auto c_renderer_ = entity_->add_id_component<Component::Renderer>("renderer", std::vector<GLuint>{2u, 2u}, 255u);

						entity_->add_id_component<Component::Trigger::Input::Camera>("camera", "camera", 64.0f * 32.0f);
						entity_->add_id_component<Component::Trigger::Input::Controller>("controller", "controller");
						entity_->add_id_component<Component::Trigger::Input::Shader>("shader_manager", "shader_manager");
						entity_->add_id_component<Component::Trigger::Input::Texture>("texture_manager", "texture_manager");
						entity_->add_id_component<Component::Trigger::Input::Font>("gilsans", "gilsans", "resources/data/gilsans.json");
						entity_->add_id_component<Component::Trigger::Input::CollisionWorld>("collision_world", "collision_world");
						
						auto e_game_objs = entity_->add_id_child("game_objs");

						e_game_objs->add_id_component<Component::Trigger::Input::GameObj::TileMap>("tilemap", "tilemap", "resources/data/TestTileMapGJK.json");
						e_game_objs->add_id_component<Component::Trigger::Input::GameObj::Player>("player", "player", (GLfloat)Game::width, 792.0f);
						e_game_objs->add_id_component<Component::Trigger::Input::GameObj::ColliderMap>("collider_map", "collider_map");
					}
				};
			}
		}
	}
}
