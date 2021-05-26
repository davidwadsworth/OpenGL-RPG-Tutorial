#include "component_bitmap_font.h"

std::vector<Component::Transform*> Component::BitMapFont::generate_word(std::string word, Component::Transform** transforms, GLuint char_limit)
{
    return std::vector<Component::Transform*>();
}

std::vector<Component::Transform*> Component::BitMapFont::generate_line(char* message, char delim, Component::Transform& textbox, float align_x, float pad_x, Component::Transform** transforms, GLuint char_lim)
{
    return std::vector<Component::Transform*>();
}

std::vector<Component::Transform*> Component::BitMapFont::generate_message(std::string message, Component::Transform& textbox, glm::vec2 align, glm::vec2 pad_x, std::vector<Component::Transform*> transforms)
{
    return std::vector<Component::Transform*>();
}
