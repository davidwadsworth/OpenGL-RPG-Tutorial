#pragma once
#include "component_rect.h"
#include <glad/glad.h>
#include <vector>

/**
Src component for bitmap fonts

@author David Wadsworth
*/

namespace Component {
	namespace Rectangle
	{
		class BitMapGlyph : public Component::Rectang
		{
		public:

			char glyph;
			int advance;
			int x_off, y_off;
			std::vector<std::pair<int, int>> kerning;

			GLuint check_kerning(char glyph);
		};
	}
}

