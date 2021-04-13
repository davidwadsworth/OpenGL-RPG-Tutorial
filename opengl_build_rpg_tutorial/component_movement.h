#pragma once
#include "component.h"
#include <glm/vec2.hpp>
#include <glad/glad.h>

namespace Component
{
	class Movement : public Comp
	{
	public:
		Movement(GLfloat speed)
			: speed(speed)
		{}

		GLfloat speed;
	};
}

