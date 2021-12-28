#pragma once
#pragma once
#include "component_system.h"
#include "component_transform.h"
#include "game.h"
#include "component_tile_map.h"
#include "component_vector.h"

/*
executes systems stored in system array based on where the camera is located assuming a tiled array of stored systems

@author David Wadsworth
*/

namespace Component {
	namespace System {
		namespace Render
		{
			class TileMap : public Component::ISystem
			{
				Component::Transform& camera_transform_;
				Component::TileMap& tile_map_;
				Component::SystemVector& render_systems_;
			public:
				TileMap(Component::Transform& camera_transform, Component::TileMap& tile_map, Component::SystemVector& render_systems)
					: camera_transform_(camera_transform), tile_map_(tile_map), render_systems_(render_systems)
				{}

				void execute() override final
				{
					const unsigned col_start = camera_transform_.x / tile_map_.tile_size;
					const unsigned row_start = camera_transform_.y / tile_map_.tile_size;

					auto tile_size = tile_map_.tile_size;
					auto map_width = tile_map_.width;
					auto map_height = tile_map_.height;

					unsigned col_end = col_start + Game::width / tile_size + (Game::width % tile_size != 0);
					col_end = col_end >= map_width ? map_width - 1 : col_end;

					unsigned row_end = row_start + Game::height / tile_size + (Game::height % tile_size != 0);
					row_end = row_end >= map_height ? map_height - 1 : row_end;

					for (auto i = row_start * map_width; i <= row_end * map_width; i += map_width)
						for (auto j = i + col_start; j <= i + col_end; j++)
							render_systems_[j]->execute();
				}
			};
		}
	}
}

