#pragma once
#include <glad/glad.h>
#include <glm/vec2.hpp>

/*
@author David Wadsworth
*/

struct Rect : public glm::vec2
{
	Rect(const Rect& rect)
	{
		set(rect);
	}

	Rect()
		: glm::vec2(0.0f), w(0.0f), h(0.0f)
	{}

	Rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h)
		: glm::vec2(x,y), w(w), h(h)
	{}

	Rect (GLfloat x, GLfloat y, GLfloat l)
		: glm::vec2(x,y), w(l), h(l)
	{}

	Rect(glm::vec2 pos, GLfloat l)
		: glm::vec2(pos), w(l), h(l)
	{}

	Rect(glm::vec2 pos)
		: glm::vec2(pos), w(0.0f), h(0.0f)
	{}

	Rect& operator=(const Rect& rect)
	{
		set(rect);
	}

	Rect& operator=(Rect&& rect) noexcept
	{
		set(rect);
	}

	Rect& operator*(float sc)
	{
		this->w *= sc;
		this->h *= sc;
		return *this;
	}

	void set(float x, float y, float w, float h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	void set(const Rect& rect)
	{
		set(rect.x, rect.y, rect.w, rect.h);
	}

	void set(Rect&& rect)
	{
		set(rect.x, rect.y, rect.w, rect.h);
	}

	GLfloat w, h;
};