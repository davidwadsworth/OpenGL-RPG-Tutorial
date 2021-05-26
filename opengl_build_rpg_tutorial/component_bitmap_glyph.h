#pragma once
#include "component.h"
#include <glad/glad.h>

namespace Component
{
	class BitMapGlyph : public IComponent
	{
	public:
		BitMapGlyph() = default;
		char glyph;
		GLuint line_h;
		GLuint child_off;
	};
}

