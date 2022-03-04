#pragma once
#include "component_trigger_input.h"
#include "component_json.h"
#include "component_texunit.h"

/*
Tileset game object from loaded in json

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class TileSet : public Component::Trigger::IInput
			{
				void create(Entity* gamestate) override final
				{
					auto tileset_json = gamestate->get_child("index")->get_component<Component::Json>(name_)->json;

					float margin = tileset_json["margin"];
					float spacing = tileset_json["spacing"];

					std::string image_src = tileset_json["image"];

					// splits the src getting the last element of 
					auto image_name = delimiter_split(delimiter_split(image_src.c_str(), '/').back().c_str(), '.')[0];

					float tile_size = tileset_json["tilewidth"];
					float tileset_w = tileset_json["imagewidth"];
					float tileset_h = tileset_json["imageheight"];

					auto& c_tileset_tex = *gamestate->get_child("texture")->get_component<Component::Texture>(image_name);
					auto& c_sprite_shader = *gamestate->get_child("shader")->get_component<Component::Shader>("sprite");

					auto& c_texunit = *gamestate->get_component<Component::TexUnit>("texunit");

					// set up tile map material
					auto& c_tset_material = *entity_->add_id_component<Component::Material>
						("material", c_tileset_tex, c_sprite_shader, c_texunit.get_open_tex_unit());

					auto e_tiles = entity_->add_id_child("tiles");
					// set up tile srcs starting at the margin and incrementing with tile size and spacing
					
					for (auto y = margin; y < tileset_h; y += tile_size + spacing)
					{
						for (auto x = margin; x < tileset_w; x += tile_size + spacing)
						{
							e_tiles->push_back_component<Component::Src>(Rect{ x, y, tile_size, tile_size });
						}
					}
				}
			public:
				using Component::Trigger::IInput::IInput;
			};
		}
	}
}