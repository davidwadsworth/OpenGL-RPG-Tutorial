#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/vec4.hpp>
#include "component_material.h"
#include "rect.h"
#include "entity.h"
/*
Simple batch renderer for drawing sprites 

@author David Wadsworth
*/
namespace Component
{
	class Renderer : public IComponent
	{
		GLuint vbo_, vao_, att_size_, max_sprites_;
		std::vector<GLfloat> buffer_;
		Component::Material* current_mat_;
		std::string name_;
	public:
		Renderer(std::string name_);
		~Renderer() { release(); }

		void init(Entity* gamestate);
		void release();

		void begin();
		void draw(Rect& src, Rect& dest, Component::Material& mat);
		void flush();
		void end();
	};
}
