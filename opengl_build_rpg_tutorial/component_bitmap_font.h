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
		Entity* font_;
	public:
		static struct Alignment
		{
			const float left = 0.0f;
			const float right = 1.0f;
			const float center = 0.5f;
		};

		struct MessageContainer
		{
			std::size_t length;
			std::vector<Component::Transform*> dests;
			std::string w;

			MessageContainer& operator+=(const MessageContainer& mc)
			{
				dests.insert(this->dests.end(), mc.dests.begin(), mc.dests.end());
				this->length += mc.length;
				w += mc.w;
				return *this;
			}
		};

		enum Spacing
		{
			DoubleSpaced,
			OnePointFiveSpaced,
			OneSpaced,
			PointFiveSpaced
		};

		BitMapFont(Entity* font, GLuint font_size, Spacing spacing)
			: font_(font)
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

		MessageContainer generate_word(std::string word, std::vector<Component::Transform*> transforms, GLuint message_pos = 0u);
		MessageContainer generate_line(char* message, char delim, Component::Transform& textbox, 
			float align_x, float pad_x, std::vector<Component::Transform*> transforms, GLuint message_pos = 0u);
		MessageContainer generate_message(std::string message, Component::Transform& textbox,
			glm::vec2 align, glm::vec2 pad_x, std::vector<Component::Transform*> transforms, GLuint message_pos = 0u);
	};
}
