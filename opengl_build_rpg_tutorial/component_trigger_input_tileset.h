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
			class TileSet : public Component::Trigger::In
			{
				std::string path_;
			public:
				TileSet(std::string name, std::string path)
					: Component::Trigger::In(name), path_(path)
				{}

			private:
				void create(EntityMap& map) override final
				{
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
						std::cerr << "Failed to read tileset file!" << std::endl;
						throw;
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

					if (!map["texture manager"]->has_component<Component::Texture>(image_name))
						map["texture manager"]->add_component_str_id<Component::Texture>(image_name)->load(image_src.c_str());
					
					auto& c_tileset_tex = *map["texture manager"]->get_component<Component::Texture>(image_name);
					auto& c_sprite_shader = *map["shader manager"]->get_component<Component::Shader>("sprite");

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