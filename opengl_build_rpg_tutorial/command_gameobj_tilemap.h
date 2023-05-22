#pragma once
#include "component_texture.h"
#include "component_shader.h"
#include "component_material.h"
#include "component_vector.h"
#include "json.hpp"
#include <fstream>
#include <sstream>
#include "delimiter_split.h"
#include "command.h"
#include "component_system_render_tilemap.h"
#include "component_engine.h"
#include "component_json.h"
/*
Creates the tile map for our player to move around in the overworld using json tiled files

@author David Wadsworth
*/

namespace Command {
	namespace GameObj
	{
		class TileMap : public ICommand
		{
			std::string name_;
			int tilemap_w_;
			int tilemap_h_;
			int tile_size_;
			float tile_scale_;
			float render_group_;

			std::string tileset_source_;
			int first_gid_;
			nlohmann::json layers_;
		public:
			void load(nlohmann::json json) override final
			{
				tilemap_w_ = json["width"];
				tilemap_h_ = json["height"];
				tile_size_ = json["tilewidth"];
				tile_scale_ = json["scale"];
				render_group_ = json["render_group"];

				tileset_source_ = json["tilesets"][0]["source"];
				first_gid_ = json["tilesets"][0]["firstgid"];
				layers_ = json["layers"];
			}

			void execute(Entity* gamestate) override final
			{
				auto e_tilemap = gamestate->add_id_child(name_);

				auto set_name = delimiter_split(delimiter_split(tileset_source_.c_str(), '/').back().c_str(), '.').front();
				// get a list of all srcs used
				auto e_tileset = gamestate->get_child(set_name)->get_child("tiles");
				auto& c_tset_material = *gamestate->get_child(set_name)->get_component<Component::Material>("material");

				// get used game objects
				auto& c_cam_position = *gamestate->get_child("camera")->get_component<Component::Position>();
				auto& c_renderer = *gamestate->get_component<Component::Renderer>("renderer");

				auto e_tiles = e_tilemap->add_id_child("tiles");

				auto& c_render_vector = *e_tilemap->add_component<Component::SystemVector>();

				std::vector<Component::Transform*> trans_vec;
				std::vector<Component::Src*> src_vec;
						
				for (auto &layer : layers_)
				{
					std::vector<int> tiles = layer["data"];
					// set up tiles
					for (auto i = 0; i < tiles.size(); ++i)
					{
						if (!tiles[i])
							continue;

						Rect tile_dest
						{
							static_cast<GLfloat>((i % tilemap_w_) * tile_size_ * tile_scale_),  // finds place in column and multiplies by sprite width
							static_cast<GLfloat>((i / tilemap_w_) * tile_size_ * tile_scale_),  // finds place in row and multiples by sprite height
							static_cast<GLfloat>(tile_size_ * tile_scale_), static_cast<GLfloat>(tile_size_ * tile_scale_)
						};
						trans_vec.push_back(e_tiles->push_back_component<Component::Transform>(tile_dest));

						// get the approriate src tile from the list of tile_srcs minus the first gid
						src_vec.push_back(e_tileset->get_component<Component::Src>(tiles[i] - first_gid_));
					}
				}

				auto csr_tmap_render = e_tilemap->add_component<Component::System::Render::TileMap>(c_cam_position,
					tilemap_w_, tilemap_h_, tile_size_ * tile_scale_, src_vec, trans_vec, c_tset_material, c_renderer);

				gamestate->get_component<Component::Engine>("render")->add(csr_tmap_render, render_group_);
			}
		};
	}
}
