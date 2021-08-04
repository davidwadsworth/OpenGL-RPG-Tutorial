#pragma once
#include "component_trigger_input.h"
#include "component_trigger_input_camera.h"
#include "component_trigger_input_controller.h"
#include "component_trigger_input_engine.h"
#include "component_trigger_input_renderer.h"
#include "component_trigger_input_shader.h"
#include "component_trigger_input_texture.h"
#include "component_trigger_input_tilemap.h"
#include "component_trigger_input_player.h"
#include "component_trigger_input_collision_world.h"
#include "component_trigger_input_collider_map.h"
#include "component_trigger_input_font.h"
#include "component_trigger_input_display_text.h"

/*
Set up class for all game object creation within the overworld state

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace GameState
			{
				class Overworld : public Component::Trigger::IInput
				{
				public:
					Overworld(std::string name)
						: Component::Trigger::IInput(name)
					{}

				private:
					void create(Entity* gamestate) override final
					{
						entity_->push_back_component<Component::Trigger::Input::Engine>("engine");
						entity_->push_back_component<Component::Trigger::Input::Camera>("camera", 64.0f * 32.0f);
						entity_->push_back_component<Component::Trigger::Input::Controller>("controller");
						entity_->push_back_component<Component::Trigger::Input::Renderer>("renderer");
						entity_->push_back_component<Component::Trigger::Input::Shader>("shader manager");
						entity_->push_back_component<Component::Trigger::Input::Texture>("texture manager");
						entity_->push_back_component<Component::Trigger::Input::Font>("gilsans", "resources/data/gilsans.json");
						entity_->push_back_component<Component::Trigger::Input::CollisionWorld>("collision world");
						entity_->push_back_component<Component::Trigger::Input::TileMap>("tilemap", "resources/data/TestTileMapGJK.json");
						entity_->push_back_component<Component::Trigger::Input::Player>("player", (GLfloat)Game::width, 792.0f);
						entity_->push_back_component<Component::Trigger::Input::ColliderMap>("collider map");
						entity_->push_back_component<Component::Trigger::Input::DisplayText>("display", "Hello world!", glm::vec2(0.0f, 0.0f));
					}
				};
			}
		}
	}
}
