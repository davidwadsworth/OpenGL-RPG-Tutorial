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
Creates tile map for our player to move around in

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
						std::cerr << "Failed to read shader files!" << std::endl;
						throw;
					}

					auto tilemap_json = nlohmann::json::parse(tm_stream);

					int tilemap_w = tilemap_json["width"];

					std::vector<int> tiles = tilemap_json["layers"][0]["data"];

					auto& tilesets = tilemap_json["tilesets"];
					std::vector<Comp*> tile_srcs;

					for (auto& set : tilesets)
					{
						std::string source = set["source"];
						auto set_name = delimiter_split(source.c_str(), '.').front();

						if (map.find(set_name) == map.end())
						{
							auto& cti_tileset = *Game::global_objects["overworld"]->
								push_back_component<Component::Trigger::Input::TileSet>(set_name, source);
							cti_tileset.execute(map);
						}

						auto curr_srcs = map[set_name]->get_component_list();
						tile_srcs.insert(tile_srcs.end(), curr_srcs.begin(), curr_srcs.end());
					}

					auto& c_grass_tex = *map["texture manager"]->get_component<Component::Texture>("grass");
					auto& c_sprite_shader = *map["shader manager"]->get_component<Component::Shader>("sprite");

					auto& c_cam_transform = *map["camera"]->get_component<Component::Transform>();

					auto& c_tmap_material = *entity_->add_component<Component::Material>(c_grass_tex, c_sprite_shader, 1);

					auto map_tiles = new Entity();
					entity_->push_back_child(map_tiles);

					auto& render_systems = *map["engine"]->get_component<Component::Vector<Component::Sys*>>("render");

					auto& c_renderer = *map["renderer"]->get_component<Component::Renderer>();

					// set up tiles
					for (auto i = 0; i < tiles.size(); ++i)
					{
						Rect grass_dest
						{
							(i % tilemap_w) * 64.0f,  // finds place in column and multiplies by sprite width
							(i / tilemap_w) * 64.0f,  // finds place in row and multiples by sprite height
							64.0f, 64.0f
						};
						auto& c_tile_transform = *map_tiles->push_back_component<Component::Transform>(grass_dest);
						auto& c_tile_src = *static_cast<Component::Src*>(tile_srcs[tiles[i] - 1]);
						auto& c_tile_dest = *map_tiles->push_back_component<Component::Dest>();
						auto csr_tile_dynamic_draw = map_tiles->push_back_component<Component::System::Render::CameraDraw>(c_renderer, c_tile_src, c_tile_dest, c_tmap_material, c_tile_transform, c_cam_transform);
						render_systems.push_back(csr_tile_dynamic_draw);
					}
				}
			};
		}
		
	}
}
