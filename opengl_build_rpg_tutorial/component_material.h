#pragma once
#include "component_texture.h"
#include "component_shader.h"

/*
Abstract holding class for texture shader combinations used in component_renderer

@author David Wadsworth
*/
namespace Component
{
	class Material : public Comp
	{
		static GLuint id_gen;
		GLint tex_unit_;
	public:
		Texture& texture;
		Shader& shader;
		GLuint id;

		Material(Texture& texture, Shader& shader, GLint tex_unit)
			: texture(texture), shader(shader), id(id_gen++), tex_unit_(tex_unit)
		{}

		~Material()
		{}

		void compile();
		void bind();
	};
}