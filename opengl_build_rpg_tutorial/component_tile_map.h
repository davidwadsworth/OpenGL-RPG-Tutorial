#pragma once
#include "component.h"
#include <glad/glad.h>

namespace Component
{
	class TileMap : public IComponent
	{
	public:
		GLuint width, height, tile_size;
	};
}

