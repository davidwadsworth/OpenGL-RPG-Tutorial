#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/vec4.hpp>
#include "component_material.h"
#include "component_src.h"
#include "component_dest.h"

/*
Simple batch renderer for drawing sprites 

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
		~Renderer() { release(); }

		void release();

		void begin();
		void draw(Component::Src& src, Component::Dest& dest, Component::Material& mat);
		void flush();
		void end();
	};
}
