#pragma once
#include "component_trigger_input.h"
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

/*
Creates the tile map for our player to move around in the overworld using json tiled files

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class TileMap : public Component::Trigger::In
			{
				std::string path_;
			public:
				TileMap(std::string name, std::string path)
					: Component::Trigger::In(name), path_(path)
				{}

			private:
				void create(EntityMap& map) override final
				{
					// load shaders from file
					std::stringstream tm_stream;
					
					try
					{
						// open files
						std::ifstream tm_file(path_);

						// read into temp string streams
						tm_stream << tm_file.rdbuf();

						// close file streams
						tm_file.close();
					}
					catch (std::exception e)
					{
						std::cerr << "Failed to read tilemap file!" << std::endl;
						throw;
					}

					// parse into json obj
					auto tilemap_json = nlohmann::json::parse(tm_stream);

					int tilemap_w = tilemap_json["width"];
					std::vector<int> tiles = tilemap_json["layers"][0]["data"];


					// set up tileset 
					// This can have multiple tilesets, and thus multiple materials
					// however for now we are going to stick with just one
					std::string tileset_source = tilemap_json["tilesets"][0]["source"];

					auto set_name = delimiter_split(tileset_source.c_str(), '.').front();

					// if not set up already add tileset to overworld gamobjects
					if (map.find(set_name) == map.end())
					{
						auto& cti_tileset = *Game::global_objects["overworld"]->
							push_back_component<Component::Trigger::Input::TileSet>(set_name, tileset_source);
						cti_tileset.execute(map);
					}

					// get a list of all srcs used
					auto tile_srcs = map[set_name]->get_component_list();
					auto& c_tset_material = *static_cast<Component::Material*>(tile_srcs[0]);
					
					// get used game objects
					auto& c_cam_transform = *map["camera"]->get_component<Component::Transform>();
					auto& render_systems = *map["engine"]->get_component<Component::Vector<Component::Sys*>>("render");
					auto& c_renderer = *map["renderer"]->get_component<Component::Renderer>();

					// set up tiles
					for (auto i = 0; i < tiles.size(); ++i)
					{
						Rect tile_dest
						{
							(i % tilemap_w) * 64.0f,  // finds place in column and multiplies by sprite width
							(i / tilemap_w) * 64.0f,  // finds place in row and multiples by sprite height
							64.0f, 64.0f
						};
						auto& c_tile_transform = *entity_->push_back_component<Component::Transform>(tile_dest);
						// get the approriate src tile from the list of tile_srcs subtracted by one
						auto& c_tile_src = *static_cast<Component::Src*>(tile_srcs[tiles[i]]);
						auto& c_tile_dest = *entity_->push_back_component<Component::Dest>();
						auto csr_tile_dynamic_draw = entity_->push_back_component<Component::System::Render::CameraDraw>
							(c_renderer, c_tile_src, c_tile_dest, c_tset_material, c_tile_transform, c_cam_transform);
						render_systems.push_back(csr_tile_dynamic_draw);
					}
				}
			};
		}
		
	}
}
