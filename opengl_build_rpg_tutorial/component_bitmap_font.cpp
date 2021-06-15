#include "component_bitmap_font.h"
#include "component_bitmap_glyph.h"

Component::BitMapFont::MessageContainer Component::BitMapFont::generate_word(std::string word, std::vector<Component::Transform*> transforms, GLuint message_pos = 0u)
{
    MessageContainer m_cont;

	GLint prev_char = -1;

	for (auto c : word)
	{
		auto font_char = font_->get_child(c);
		
		auto& c_bitmap_char = *font_char->get_component<Component::BitMapGlyph>();

		m_cont.length += c_bitmap_char.advance + c_bitmap_char.check_kerning(prev_char);
		prev_char = c;
		m_cont.w.push_back(prev_char);
	}

    return m_cont;
}

Component::BitMapFont::MessageContainer Component::BitMapFont::generate_line(char* message, char delim, Component::Transform& textbox, float align_x, float pad_x, std::vector<Component::Transform*> transforms, GLuint message_pos)
{
	MessageContainer m_cont;



	return MessageContainer();
}

Component::BitMapFont::MessageContainer Component::BitMapFont::generate_message(std::string message, Component::Transform& textbox, glm::vec2 align, glm::vec2 pad_x, std::vector<Component::Transform*> transforms, GLuint message_pos)
{
	return MessageContainer();
}