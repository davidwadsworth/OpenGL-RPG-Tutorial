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
			class TileMap : public Component::Trigger::IInput
			{
				std::string path_;
			public:
				TileMap(std::string name, std::string path)
					: Component::Trigger::IInput(name), path_(path)
				{}

			private:
				void create(Entity* gamestate) override final
				{
					// load tilemap from file
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
						Logger::error("Failed to read tilemap file! path = " + path_, Logger::MEDIUM);
						return;
					}

					// parse into json obj
					auto tilemap_json = nlohmann::json::parse(tm_stream);

					int tilemap_w = tilemap_json["width"];
					std::vector<int> tiles = tilemap_json["layers"][0]["data"];


					// set up tileset 
					// This can have multiple tilesets, and thus multiple materials
					// however for now we are going to stick with just one
					std::string tileset_source = tilemap_json["tilesets"][0]["source"];

					auto set_name = delimiter_split(delimiter_split(tileset_source.c_str(), '/').back().c_str(), '.').front();

					// if not set up already add tileset to overworld gamobjects
					if (!gamestate->has_child(set_name))
					{
						auto tileset = new Entity();
						auto& cti_tileset = *tileset->add_id_component<Component::Trigger::Input::TileSet>(set_name, set_name, tileset_source);
						cti_tileset.execute(gamestate);

						entity_->push_back_child(tileset);
					}

					// get a list of all srcs used
					auto tile_srcs = gamestate->get_child(set_name)->get_component_list();
					auto& c_tset_material = *static_cast<Component::Material*>(tile_srcs[0]);
					
					// get used game objects
					auto& c_cam_transform = *gamestate->get_child("camera")->get_component<Component::Transform>();
					auto& render_systems = *gamestate->get_child("engine")->get_component<Component::SystemVector>("render");
					auto& c_renderer = *gamestate->get_child("renderer")->get_component<Component::Renderer>();

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
						auto csr_tile_dynamic_draw = entity_->push_back_component<Component::System::Render::CameraDraw>
							(c_renderer, c_tile_src, c_tile_transform, c_tset_material, c_cam_transform);
						render_systems.push_back(csr_tile_dynamic_draw);
					}
				}
			};
		}
		
	}
}
