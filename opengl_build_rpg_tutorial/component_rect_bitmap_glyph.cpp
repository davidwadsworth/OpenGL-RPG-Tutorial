#include "component_rect_bitmap_glyph.h"

GLuint Component::Rectangle::BitMapGlyph::check_kerning(char glyph_2)
{
	GLuint r_kerning = 0u;

	if (glyph_2 < 0)
		return r_kerning;

	for (auto k : this->kerning)
		if (k.first == glyph_2)
		{
			r_kerning = k.second;
			break;
		}

	return r_kerning;
}
