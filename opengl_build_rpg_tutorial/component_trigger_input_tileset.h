#pragma once
#include "component_trigger_input.h"
#include "json.hpp"

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
				std::string path_;
			public:
				TileSet(std::string name, std::string path)
					: Component::Trigger::IInput(name), path_(path)
				{}

			private:
				void create(Entity* gamestate) override final
				{
					gamestate->get_child("observer")->add_id_component<Component::SystemObserver>("tileset");

					// load shaders from file
					std::stringstream ts_stream;

					try
					{
						// open files
						std::ifstream ts_file(path_);

						// read into temp string streams
						ts_stream << ts_file.rdbuf();

						// close file streams
						ts_file.close();
					}
					catch (std::exception e)
					{
						Logger::error("Failed to read tileset file! path = " + path_, Logger::MEDIUM);
						return;
					}

					auto tileset_json = nlohmann::json::parse(ts_stream);

					float margin = tileset_json["margin"];
					float spacing = tileset_json["spacing"];

					std::string image_src = tileset_json["image"];

					// splits the src getting the last element of 
					auto image_name = delimiter_split(delimiter_split(image_src.c_str(), '/').back().c_str(), '.')[0];

					float tile_size = tileset_json["tilewidth"];
					float tileset_w = tileset_json["imagewidth"];
					float tileset_h = tileset_json["imageheight"];

					if (!gamestate->get_child("texture manager")->has_component<Component::Texture>(image_name))
						gamestate->get_child("texture manager")->add_id_component<Component::Texture>(image_name)->load(image_src.c_str());
					
					auto& c_tileset_tex = *gamestate->get_child("texture manager")->get_component<Component::Texture>(image_name);
					auto& c_sprite_shader = *gamestate->get_child("shader manager")->get_component<Component::Shader>("sprite");

					// set up tile map material
					auto& c_tset_material = *entity_->push_back_component<Component::Material>(c_tileset_tex, c_sprite_shader, 1);

					// set up tile srcs starting at the margin and incrementing with tile size and spacing
					for (auto y = margin; y < tileset_h; y += tile_size + spacing)
					{
						for (auto x = margin; x < tileset_w; x += tile_size + spacing)
						{
							entity_->push_back_component<Component::Src>(Rect{ x, y, tile_size, tile_size });
						}
					}
				}
			};
		}
	}
}