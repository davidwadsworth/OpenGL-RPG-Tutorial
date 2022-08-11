#pragma once
#include "component_trigger_input.h"
#include "component_trigger_input_camera.h"
//#include "component_trigger_input_controller.h"
#include "component_trigger_input_shader.h"
#include "component_trigger_input_texture.h"
#include "component_trigger_input_gameobj_tilemap.h"
#include "component_trigger_input_gameobj_player.h"
#include "component_trigger_input_quadtree.h"
#include "component_trigger_input_gameobj_collidermap.h"
#include "component_trigger_input_font.h"
#include "component_trigger_input_gameobj_text.h"
#include "component_trigger_input_gamestate.h"
#include "component_engine.h"
#include "component_trigger_input_textbox.h"
#include "component_trigger_input_index.h"
#include "component_trigger_input_loadcache.h"
#include "component_trigger_input_spritesheet.h"
#include "component_texunit.h"

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
					Component::SystemVector* c_update_;
					std::vector<Component::ISystem*> render_;
					Component::TriggerVector* c_triggers_ = nullptr;
					Component::Renderer* c_renderer_ = nullptr;
					std::vector<Component::Trigger::IInput*> objects_;
					Component::Engine* c_render_engine_, * c_update_engine_;
					
				public:
					using Component::Trigger::Input::IGameState::IGameState;

					void init() override final
					{
						// init game objs
						c_renderer_->init(entity_);

						int i = 0;
						for (auto obj : objects_)
						{
							auto input = static_cast<Component::Trigger::IInput*>(obj);
							input->execute(entity_);
						}
						c_render_engine_ = entity_->get_component<Component::Engine>("render");
						c_update_engine_ = entity_->get_component<Component::Engine>("update");

						auto flat_u_engine = c_update_engine_->flatten();
						c_update_->insert(c_update_->begin(), flat_u_engine.begin(), flat_u_engine.end());
						render_ = c_render_engine_->flatten();
					}

					void destroy() override final
					{
						c_renderer_->release();
						// destroy all active game objs
						entity_->clear_children();
						render_.clear();
						c_update_->clear();
						c_render_engine_->clear();
						c_update_engine_->clear();
					}

					void run() override final
					{
						// make updates to live entities
						for (auto u : *c_update_)
							u->execute();

						c_renderer_->begin();

						// make draw calls to renderer
						for (auto r : render_)
							r->execute();

						c_renderer_->end();

						for (auto i = 0; i < c_triggers_->size(); ++i)
							c_triggers_->at(i)->execute(entity_);
						c_triggers_->clear();
					}

				private:
					void create(Entity* gamestate) override final
					{
						// set up renderer and engine systems for run
						entity_->add_id_component<Component::Engine>("render");
						entity_->add_id_component<Component::Engine>("update");
						
						c_update_ = entity_->add_id_component<Component::SystemVector>("update_vec");
						c_triggers_ = entity_->add_id_component<Component::TriggerVector>("trigger");

						c_renderer_ = entity_->add_id_ct_input<Component::Renderer>("renderer");
						entity_->add_id_component<Component::TexUnit>("texunit");

						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::Index>("index", "index_overworld.json"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::LoadCache>("load"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::Camera>("camera"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::Shader>("shader"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::Texture>("texture"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::Font>("font"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::PhysicsActionGJKQTree>("physics_action_qtree"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::ActionGJKQTree>("action_qtree"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::TileSet>("tileset_outside"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::TileSet>("tileset_colliders"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::SpriteSheet>("spritesheet"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::SpriteSheet>("spritesheet_overworld"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::GameObj::TileMap>("tilemap"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::GameObj::Player>("player"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::GameObj::ColliderMap>("collidermap"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::TextBox>("textbox"));

					}
				};
			}
		}
	}
}
