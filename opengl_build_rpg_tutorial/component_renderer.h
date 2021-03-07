#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/vec4.hpp>
#include "component_material.h"
#include "rect.h"

/*
Simple batch renderer for drawing sprites from the same image and shader

@author David Wadsworth
*/
namespace Component
{
	class Renderer : public Comp
	{
		GLuint vbo_, vao_, att_size_, max_sprites_;
		std::vector<GLfloat> buffer_;
		Component::Material* current_mat_;
	public:
		Renderer(std::vector<GLuint> attributes, GLuint max_sprites);
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&& other) noexcept;

		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&& other) noexcept;

		~Renderer() { release(); }

		void release();

		void begin();
		void draw(Rect src, Rect dest, Component::Material& mat);
		void flush();
		void end();
	};
}
