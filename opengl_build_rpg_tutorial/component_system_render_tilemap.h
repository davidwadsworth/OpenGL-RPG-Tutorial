#pragma once
#pragma once
#include "component_system_item.h"
#include "component_transform.h"
#include "game.h"
#include "component_tilemap.h"
#include "component_vector.h"

/*
executes systems stored in system array based on where the camera is located assuming a tiled array of stored systems

@author David Wadsworth
*/

namespace Component {
	namespace System {
		namespace Item
		{
			class TileMap : public Component::System::IItem
			{
				Component::Transform& camera_transform_;
				Component::TileMap& tilemap_;
			public:
				TileMap(Component::Transform& camera_transform, Component::TileMap& tilemap, std::vector<Component::ISystem*> systems)
					: Component::System::IItem(systems), camera_transform_(camera_transform), tilemap_(tilemap)
				{}

				void execute() override final
				{
					const unsigned col_start = camera_transform_.x / tilemap_.tile_size;
					const unsigned row_start = camera_transform_.y / tilemap_.tile_size;

					auto tile_size = tilemap_.tile_size;
					auto map_width = tilemap_.width;
					auto map_height = tilemap_.height;

					unsigned col_end = col_start + Game::width / tile_size + (Game::width % tile_size != 0);
					col_end = col_end >= map_width ? map_width - 1 : col_end;

					unsigned row_end = row_start + Game::height / tile_size + (Game::height % tile_size != 0);
					row_end = row_end >= map_height ? map_height - 1 : row_end;

					for (auto i = row_start * map_width; i <= row_end * map_width; i += map_width)
						for (auto j = i + col_start; j <= i + col_end; j++)
							systems_[j]->execute();
				}
			};
		}
	}
}

