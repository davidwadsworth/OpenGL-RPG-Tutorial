#pragma once
#include "component_trigger_input.h"
#include "component_trigger_input_camera.h"
#include "component_trigger_input_controller.h"
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
#include "component_trigger_input_gameobj_textbox.h"
#include "component_trigger_input_index.h"
#include "component_trigger_input_loadcache.h"

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
					
				public:
					using Component::Trigger::Input::IGameState::IGameState;

					void init() override final
					{
						// init game objs
						for (auto obj : objects_)
							static_cast<Component::ITrigger*>(obj)->execute(entity_);

						c_renderer_->init(entity_);
					}

					void destroy() override final
					{
						c_renderer_->release();
						// destroy all active game objs
						entity_->clear_children();
						render_.clear();
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
						auto& c_render_engine = *entity_->add_id_component<Component::Engine>("render_engine");
						auto& c_update_engine = *entity_->add_id_component<Component::Engine>("update_engine");
						
						c_update_ = entity_->add_id_component<Component::SystemVector>("update_vec");
						c_triggers_ = entity_->add_id_component<Component::TriggerVector>("trigger");

						c_renderer_ = entity_->add_id_ct_input<Component::Renderer>("renderer");
						entity_->add_id_component<Component::TexUnit>("texunit");

						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::Index>("index"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::LoadCache>("load"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::Camera>("camera"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::Controller>("controller"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::Shader>("shader_manager"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::Texture>("texture_manager"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::Font>("gilsans"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::QuadTree>("collision_qtree"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::QuadTree>("action_qtree"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::GameObj::TileMap>("tilemap"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::GameObj::Player>("player"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::GameObj::ColliderMap>("collider_map"));
						objects_.push_back(entity_->add_id_ct_input<Component::Trigger::Input::GameObj::TextBox>("textbox"));

						auto flat_u_engine = c_update_engine.flatten();
						c_update_->insert( c_update_->begin(), flat_u_engine.begin(), flat_u_engine.end());
						render_ = c_render_engine.flatten();
					}
				};
			}
		}
	}
}
