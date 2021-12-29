#pragma once
#include "component.h"
#include <glad/glad.h>

namespace Component
{
	class TileMap : public IComponent
	{
	public:
		TileMap(GLuint width, GLuint height, GLuint tile_size)
			: width(width), height(height), tile_size(tile_size)
		{}

		GLuint width, height, tile_size;
	};
}

