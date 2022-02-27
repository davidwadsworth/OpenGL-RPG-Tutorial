#pragma once
#pragma once
#include "component_system_item.h"
#include "game.h"
#include "component_vector.h"
#include "component_renderer.h"
#include "component_rect.h"
#include "component_position.h"

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
				Component::Position& camera_pos_;
				std::vector<Component::Src*> srcs_;
				std::vector<Component::Transform*> transforms_;
				Component::Material& material_;
				Component::Renderer& renderer_;
				GLuint map_width_, map_height_, tile_size_;
			public:
				TileMap(Component::Position& camera_pos, GLuint map_width, GLuint map_height, 
					GLuint tile_size, std::vector<Component::Src*> srcs, std::vector<Component::Transform*> transforms, 
					Component::Material& material, Component::Renderer& renderer)
					: camera_pos_(camera_pos), srcs_(srcs), transforms_(transforms), material_(material), 
					renderer_(renderer), map_width_(map_width), map_height_(map_height), tile_size_(tile_size)
				{}

				void execute() override final
				{
					const unsigned col_start = camera_pos_.x / tile_size_;
					const unsigned row_start = camera_pos_.y / tile_size_;

					unsigned col_end = col_start + Game::width / tile_size_ + (Game::width % tile_size_ != 0);
					col_end = col_end >= map_width_ ? map_width_ - 1 : col_end;

					unsigned row_end = row_start + Game::height / tile_size_ + (Game::height % tile_size_ != 0);
					row_end = row_end >= map_height_ ? map_height_ - 1 : row_end;

					for (auto i = row_start * map_width_; i <= row_end * map_width_; i += map_width_)
						for (auto j = i + col_start; j <= i + col_end; j++)
						{
							// update render dest by camera and local transforms
							Rect dest;

							dest.x = transforms_[j]->x - camera_pos_.x;
							dest.y = transforms_[j]->y - camera_pos_.y;
							dest.w = transforms_[j]->w;
							dest.h = transforms_[j]->h;

							renderer_.draw(*srcs_[j], dest, material_);
						}
				}
			};
		}
	}
}

