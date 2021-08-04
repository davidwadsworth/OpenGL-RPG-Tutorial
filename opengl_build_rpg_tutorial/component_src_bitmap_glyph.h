#pragma once
#include "component_src.h"
#include <glad/glad.h>
#include <vector>

/**
Src component for bitmap fonts

@author David Wadsworth
*/

namespace Component
{
	class BitMapGlyph : public Src
	{
	public:
		using Component::Src::Src;

		char glyph;
		int advance;
		int x_off, y_off;
		std::vector<std::pair<int, int>> kerning;

		GLuint check_kerning(char glyph);
	};
}

