#pragma once
#include "component_trigger_input.h"
#include "component_texture.h"
#include "component_shader.h"
#include "component_material.h"
#include "component_vector.h"
#include "component_system_render_camera_draw.h"

/*
Creates tile map for our player to move around in

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class TileMap : public Component::Trigger::In
			{
				GLuint rows_, cols_;
			public:
				TileMap(std::string name, GLuint rows, GLuint cols)
					: Component::Trigger::In(name), rows_(rows), cols_(cols)
				{}

			private:
				void create(EntityMap& map) override final
				{
					auto& c_grass_tex = *map["texture manager"]->get_component<Component::Texture>("grass");
					auto& c_sprite_shader = *map["shader manager"]->get_component<Component::Shader>("sprite");

					auto& c_cam_transform = *map["camera"]->get_component<Component::Transform>();

					auto& c_tmap_material = *entity_->add_component<Component::Material>(c_grass_tex, c_sprite_shader, 1);

					// how many total grass to draw
					auto total_grass = cols_ * rows_;

					auto tiles = new Entity();
					entity_->push_back_child(tiles);

					auto& render_systems = *map["engine"]->get_component<Component::Vector<Component::Sys*>>("render");

					auto& c_renderer = *map["renderer"]->get_component<Component::Renderer>();

					// set up tiles
					for (auto i = 0; i < total_grass; ++i)
					{
						Rect grass_dest
						{
							(i % cols_) * 64.0f,  // finds place in column and multiplies by sprite width
							(i / cols_) * 64.0f,  // finds place in row and multiples by sprite height
							64.0f, 64.0f
						};
						auto& c_tile_transform = *tiles->push_back_component<Component::Transform>(grass_dest);
						auto& c_tile_src = *tiles->push_back_component<Component::Src>(Rect{ 0.0f, 0.0f, 64.0f, 64.0f });
						auto& c_tile_dest = *tiles->push_back_component<Component::Dest>();
						auto csr_tile_dynamic_draw = tiles->push_back_component<Component::System::Render::CameraDraw>(c_renderer, c_tile_src, c_tile_dest, c_tmap_material, c_tile_transform, c_cam_transform);
						render_systems.push_back(csr_tile_dynamic_draw);
					}
				}
			};
		}
		
	}
}
