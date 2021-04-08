#pragma once
#include "component_trigger_input.h"
#include "json.hpp"

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
						std::cerr << "Failed to read shader files!" << std::endl;
						throw;
					}

					auto tileset_json = nlohmann::json::parse(ts_stream);

					float margin = tileset_json["margin"];
					float spacing = tileset_json["spacing"];

					std::string image_src = tileset_json["image"];

					float tile_size = tileset_json["tilewidth"];
					float tileset_w = tileset_json["imagewidth"];
					float tileset_h = tileset_json["imageheight"];

					for (auto y = margin; y < tileset_h; y += tile_size + spacing)
					{
						for (auto x = margin; x < tileset_w; x += tile_size + spacing)
						{
							auto src_rect = Rect{ x, y, tile_size, tile_size };
							std::cout << "rect: " << x << ", " << y << std::endl;
							entity_->push_back_component<Component::Src>(src_rect);
						}
					}
				}
			};
		}
	}
}