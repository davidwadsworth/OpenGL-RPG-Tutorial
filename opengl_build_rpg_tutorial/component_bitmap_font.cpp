#include "component_bitmap_font.h"
#include "component_bitmap_glyph.h"

std::vector<Component::Transform*> Component::BitMapFont::generate_word(std::string word, Component::Transform** transforms, GLuint char_limit)
{
    std::vector<Component::Transform*> r_word;

	GLint prev_char = -1;

	for (auto c : word)
	{
		auto font_char = font_->get_child(c);
		
		auto& c_bitmap_char = *font_char->get_component<Component::BitMapGlyph>();

		temp_word.length += c_bitmap_char.advance + c_bitmap_char.check_kerning(prev_char);
		prev_char = c;
		temp_word.w.push_back(prev_char);
	}

    return r_word;
}

std::vector<Component::Transform*> Component::BitMapFont::generate_line(char* message, char delim, Component::Transform& textbox, float align_x, float pad_x, Component::Transform** transforms, GLuint char_lim)
{
    return std::vector<Component::Transform*>();
}

std::vector<Component::Transform*> Component::BitMapFont::generate_message(std::string message, Component::Transform& textbox, glm::vec2 align, glm::vec2 pad_x, std::vector<Component::Transform*> transforms)
{
    return std::vector<Component::Transform*>();
}
