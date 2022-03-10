#pragma once
#include <glad/glad.h>
#include <glm/vec2.hpp>

/*
Defines a rectangle (x,y,w,h)

@author David Wadsworth
*/

struct Rect
{
	Rect(const Rect& rect)
		: x(rect.x), y(rect.y), w(rect.w), h(rect.h)
	{}

	Rect()
		: x(0.0f), y(0.0f), w(0.0f), h(0.0f)
	{}

	Rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h)
		: x(x), y(y), w(w), h(h)
	{}

	Rect (GLfloat x, GLfloat y, GLfloat l)
		: x(x), y(y), w(l), h(l)
	{}

	Rect(glm::vec2 pos, GLfloat l)
		: x(pos.x), y(pos.y), w(l), h(l)
	{}

	Rect(glm::vec2 pos)
		: x(pos.x), y(pos.y), w(0.0f), h(0.0f)
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

	float x,y,w, h;
};