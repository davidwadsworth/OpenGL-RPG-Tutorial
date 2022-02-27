#pragma once
#include "component_trigger_input_gameobj.h"
#include "component_texture.h"
#include "component_shader.h"
#include "component_material.h"
#include "component_vector.h"
#include "component_system_render_camera_draw.h"
#include "json.hpp"
#include <fstream>
#include <sstream>
#include "delimiter_split.h"
#include "component_trigger_input_tileset.h"
#include "component_system_render_tilemap.h"

/*
Creates the tile map for our player to move around in the overworld using json tiled files

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace GameObj
			{
				class TileMap : public Component::Trigger::Input::IGameObj
				{
				public:
					TileMap(std::string name)
						: Component::Trigger::Input::IGameObj(name)
					{}

				private:
					void init(Entity* gamestate) override final
					{
						// parse into json obj
						auto& tilemap_json = gamestate->get_child("index")->get_child(name_)->get_component<Component::Json>()->json;

						int tilemap_w = tilemap_json["width"];
						int tilemap_h = tilemap_json["height"];
						int tile_size = tilemap_json["tilesize"];
						float tile_scale = tilemap_json["scale"];
						float render_group = tilemap_json["render_group"];


						std::vector<int> tiles = tilemap_json["layers"][0]["data"];

						// set up tileset 
						// This can have multiple tilesets, and thus multiple materials
						// however for now we are going to stick with just one
						std::string tileset_source = tilemap_json["tilesets"][0]["source"];

						auto set_name = delimiter_split(delimiter_split(tileset_source.c_str(), '/').back().c_str(), '.').front();

						// if not set up already add tileset to overworld gamobjects
						if (!gamestate->has_child(set_name))
							Logger::error("index.json missing tileset: " + set_name, Logger::SEVERITY::HIGH);

						// get a list of all srcs used
						auto tile_srcs = gamestate->get_child(set_name)->get_component_list();
						auto& c_tset_material = *static_cast<Component::Material*>(tile_srcs[0]);

						// get used game objects
						auto& c_cam_position = *gamestate->get_child("camera")->get_component<Component::Position>();
						auto& c_renderer = *gamestate->get_component<Component::Renderer>("renderer");

						auto e_tiles = new Entity();

						auto& c_render_vector = *entity_->add_component<Component::SystemVector>();

						std::vector<Component::Transform*> trans_vec;
						std::vector<Component::Src*> src_vec;
						// set up tiles
						for (auto i = 0; i < tiles.size(); ++i)
						{
							Rect tile_dest
							{
								(i % tilemap_w) * tile_size,  // finds place in column and multiplies by sprite width
								(i / tilemap_w) * tile_size ,  // finds place in row and multiples by sprite height
								tile_size, tile_size
							};
							trans_vec.push_back(e_tiles->push_back_component<Component::Transform>(tile_dest, tile_scale));
							
							// get the approriate src tile from the list of tile_srcs subtracted by one
							src_vec.push_back(static_cast<Component::Src*>(tile_srcs[tiles[i]]));
						}

						auto csr_tmap_render = e_game_info_->add_id_component<Component::System::Render::TileMap>("render", c_cam_position, 
							tilemap_w, tilemap_h, tile_size * tile_scale, src_vec, trans_vec, c_tset_material, c_renderer);

						gamestate->get_component<Component::Engine>("render")->add(csr_tmap_render, render_group);
					}
				};
			}
		}
		
	}
}
