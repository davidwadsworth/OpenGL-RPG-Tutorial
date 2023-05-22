#pragma once
#include "command.h"
#include "component_json.h"
#include "component_texunit.h"
#include "component_rect.h"
#include "delimiter_split.h"
#include "component_material.h"

/*
Tileset game object from loaded in json

@author David Wadsworth
*/

namespace Command {
	namespace GameObj {
			class TileSet : public ICommand
			{
				std::string name_, image_name_;
		
				float margin_;
				float spacing_;
				
				float tile_size_;
				float tileset_w_;
				float tileset_h_;
			public:

				void load(nlohmann::json json) override final
				{
					name_ = json["name"];

					margin_ = json["margin"];
					spacing_ = json["spacing"];

					std::string image_src = json["image"];

					// splits the src getting the last element of 
					image_name_ = delimiter_split(delimiter_split(image_src.c_str(), '/').back().c_str(), '.')[0];

					tile_size_ = json["tilewidth"];
					tileset_w_ = json["imagewidth"];
					tileset_h_ = json["imageheight"];
				}

				void execute(Entity* gamestate) override final
				{
					auto e_tileset = gamestate->add_id_child(name_);

					auto& c_tileset_tex = *gamestate->get_child("texture")->get_component<Component::Texture>(image_name_);
					auto& c_sprite_shader = *gamestate->get_child("shader")->get_component<Component::Shader>("sprite");

					auto& c_texunit = *gamestate->get_component<Component::TexUnit>("texunit");

					// set up tile map material
					auto& c_tset_material = *e_tileset->add_id_component<Component::Material>
						("material", c_tileset_tex, c_sprite_shader, c_texunit.get_open_tex_unit());

					auto e_tiles = e_tileset->add_id_child("tiles");
					// set up tile srcs starting at the margin and incrementing with tile size and spacing
					
					for (auto y = margin_; y < tileset_h_; y += tile_size_ + spacing_)
					{
						for (auto x = margin_; x < tileset_w_; x += tile_size_ + spacing_)
						{
							e_tiles->push_back_component<Component::Src>(Rect( x, y, tile_size_, tile_size_ ));
						}
					}
				}
			};
		}
	}
}