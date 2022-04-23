#pragma once
#include "component_texture.h"
#include "component_shader.h"
#include "component_material.h"
#include "component_vector.h"
#include "json.hpp"
#include <fstream>
#include <sstream>
#include "delimiter_split.h"
#include "component_trigger_input_tileset.h"
#include "component_system_render_tilemap.h"
#include "component_engine.h"

/*
Creates the tile map for our player to move around in the overworld using json tiled files

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace GameObj
			{
				class TileMap : public Component::Trigger::IInput
				{
				private:
					void create(Entity* gamestate) override final
					{
						auto tilemap_name = delimiter_split(name_, '_')[0];

						// parse into json obj
						auto& tilemap_json = gamestate->get_child("index")->get_component<Component::Json>(tilemap_name)->json;

						int tilemap_w = tilemap_json["width"];
						int tilemap_h = tilemap_json["height"];
						int tile_size = tilemap_json["tilewidth"];
						float tile_scale = tilemap_json["scale"];
						float render_group = tilemap_json["render_group"];


						// set up tileset 
						// This can have multiple tilesets, and thus multiple materials
						// however for now we are going to stick with just one
						std::string tileset_source = tilemap_json["tilesets"][0]["source"];
						int first_gid = tilemap_json["tilesets"][0]["firstgid"];
						auto set_name = delimiter_split(delimiter_split(tileset_source.c_str(), '/').back().c_str(), '.').front();

						// get a list of all srcs used
						auto e_tileset = gamestate->get_child(set_name)->get_child("tiles");
						auto& c_tset_material = *gamestate->get_child(set_name)->get_component<Component::Material>("material");

						// get used game objects
						auto& c_cam_position = *gamestate->get_child("camera")->get_component<Component::Position>();
						auto& c_renderer = *gamestate->get_component<Component::Renderer>("renderer");

						auto e_tiles = entity_->add_id_child("tiles");

						auto& c_render_vector = *entity_->add_component<Component::SystemVector>();

						std::vector<Component::Transform*> trans_vec;
						std::vector<Component::Src*> src_vec;
						
						for (auto& layer : tilemap_json["layers"])
						{
							std::vector<int> tiles = layer["data"];
							// set up tiles
							for (auto i = 0; i < tiles.size(); ++i)
							{
								if (!tiles[i])
									continue;

								Rect tile_dest
								{
									static_cast<GLfloat>((i % tilemap_w) * tile_size * tile_scale),  // finds place in column and multiplies by sprite width
									static_cast<GLfloat>((i / tilemap_w) * tile_size * tile_scale),  // finds place in row and multiples by sprite height
									static_cast<GLfloat>(tile_size * tile_scale), static_cast<GLfloat>(tile_size * tile_scale)
								};
								trans_vec.push_back(e_tiles->push_back_component<Component::Transform>(tile_dest));

								// get the approriate src tile from the list of tile_srcs minus the first gid
								src_vec.push_back(e_tileset->get_component<Component::Src>(tiles[i] - first_gid));
							}
						}

						auto csr_tmap_render = entity_->add_component<Component::System::Render::TileMap>(c_cam_position,
							tilemap_w, tilemap_h, tile_size * tile_scale, src_vec, trans_vec, c_tset_material, c_renderer);

						gamestate->get_component<Component::Engine>("render")->add(csr_tmap_render, render_group);
					}
				public:
					using Component::Trigger::IInput::IInput;
				};
			}
		}

	}
}
