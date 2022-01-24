#pragma once
#pragma once
#include "component_system_item.h"
#include "component_transform.h"
#include "game.h"
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
				GLuint map_width_, map_height_, tile_size_;
			public:
				TileMap(Component::Transform& camera_transform, GLuint map_width, GLuint map_height, 
					GLuint tile_size, std::vector<Component::ISystem*> systems)
					: Component::System::IItem(systems), camera_transform_(camera_transform), 
					map_width_(map_width), map_height_(map_height), tile_size_(tile_size)
				{}

				void execute() override final
				{
					const unsigned col_start = camera_transform_.x / tile_size_;
					const unsigned row_start = camera_transform_.y / tile_size_;

					unsigned col_end = col_start + Game::width / tile_size_ + (Game::width % tile_size_ != 0);
					col_end = col_end >= map_width_ ? map_width_ - 1 : col_end;

					unsigned row_end = row_start + Game::height / tile_size_ + (Game::height % tile_size_ != 0);
					row_end = row_end >= map_height_ ? map_height_ - 1 : row_end;

					for (auto i = row_start * map_width_; i <= row_end * map_width_; i += map_width_)
						for (auto j = i + col_start; j <= i + col_end; j++)
							systems_[j]->execute();
				}
			};
		}
	}
}

