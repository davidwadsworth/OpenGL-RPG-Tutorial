#pragma once
#include "component.h"
#include <glad/glad.h>
#include "entity.h"
#include "rect.h"
#include "component_transform.h"
#include <glm/glm.hpp>
#include <array>
#include "component_src.h"

namespace Component
{
	class BitMapFont : public IComponent
	{
		GLuint line_h_;
	public:
		static struct Alignment
		{
			const float left = 0.0f;
			const float right = 1.0f;
			const float center = 0.5f;
		};

		enum Spacing
		{
			DoubleSpaced,
			OnePointFiveSpaced,
			OneSpaced,
			PointFiveSpaced
		};

		BitMapFont(GLuint font_size, Spacing spacing)
		{
			switch (spacing)
			{
			case DoubleSpaced:
				line_h_ = font_size;
				break;
			case OnePointFiveSpaced:
				line_h_ = font_size - font_size / 4;
				break;
			case OneSpaced:
				line_h_ = font_size - font_size / 3;
				break;
			case PointFiveSpaced:
				line_h_ = font_size - font_size / 2;
				break;
			default:
				line_h_ = font_size;
				break;
			}
		}

		std::vector<Component::Transform*> generate_word(std::string word, Component::Transform** transforms, GLuint char_limit);
		std::vector<Component::Transform*> generate_line(char* message, char delim, Component::Transform& textbox, 
			float align_x, float pad_x, Component::Transform** transforms, GLuint char_lim);
		std::vector<Component::Transform*> generate_message(std::string message, Component::Transform& textbox,
			glm::vec2 align, glm::vec2 pad_x, std::vector<Component::Transform*> transforms);
	};
}
