#pragma once
#include <glad/glad.h>
#include <glm/vec2.hpp>

/*
@author David Wadsworth
*/

struct Rect : public glm::vec2
{
	Rect()
		: glm::vec2(0.0f), w(0.0f), h(0.0f)
	{}

	Rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h)
		: glm::vec2(x,y), w(w), h(h)
	{}

	GLfloat w, h;
};