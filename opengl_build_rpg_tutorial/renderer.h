#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/vec4.hpp>

/*
Simple renderer for drawing sprites from the same image and shader

@author David Wadsworth
*/

#define Rect glm::vec4 // { x, y, z = width, w = height }

class Renderer
{
	GLuint vbo_, vao_, att_size_;
	std::vector<GLfloat> buffer_;
public:
	Renderer(std::vector<GLuint> attributes);
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&& other);

	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&& other);

	~Renderer() { release(); }

	void release();

	void draw(Rect dest_rect, Rect src_rect);
	void flush();
};