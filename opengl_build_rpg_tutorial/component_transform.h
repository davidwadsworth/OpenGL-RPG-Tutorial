#pragma once
#include "component.h"
#include <glm/vec2.hpp>
#include <glad/glad.h>

/*
Locally bound transformational information. Not where the entity is located but rather located relative to an invible plane
@author David Wadsworth
*/

namespace Component
{
	struct Transform : Comp
	{
		Transform(glm::vec2 position, GLint width, GLint height, GLfloat scale)
			: position(position), width(width), height(height), scale(scale)
		{}

		Transform(glm::vec2 position, GLint width, GLint height)
			: Transform(position, width, height, 1.0f)
		{}

		Transform(glm::vec2 position, GLint length)
			: Transform(position, length, length, 1.0f)
		{}

		glm::vec2 position;
		GLint width, height;
		GLfloat scale;
	};
}


