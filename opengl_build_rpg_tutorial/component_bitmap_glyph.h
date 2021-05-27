#pragma once
#include "component.h"
#include <glad/glad.h>
#include <vector>

namespace Component
{
	class BitMapGlyph : public IComponent
	{
	public:
		BitMapGlyph() = default;
		GLchar glyph;
		GLint advance;
		GLuint width, height;
		GLint x_off, y_off;
		std::vector<std::pair<GLint, GLint>> kerning;

		GLuint check_kerning(char glyph);
	};
}

