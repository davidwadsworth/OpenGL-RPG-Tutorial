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

					int margin = tileset_json["margin"];
					int spacing = tileset_json["spacing"];

					std::string image_src = tileset_json["image"];

					int tile_size = tileset_json["tilewidth"];
					int tileset_w = tileset_json["imagewidth"];
					int tileset_h = tileset_json["imageheight"];

					for (auto y = margin; y < tileset_h; y += tile_size + spacing)
					{
						for (auto x = margin; x < tileset_w; x += tile_size + spacing)
						{
							auto tile(new Entity());

							auto src = tile->add_component<Component::Src>(
								Rect{ x / (GLfloat)tileset_w, y / (GLfloat)tileset_h,
									(x + tile_size) / (GLfloat)tileset_w, (y + tile_size) / (GLfloat)tileset_h }
							);

							entity_->push_back_child(tile);
						}
					}

				}
			};
		}
	}
}