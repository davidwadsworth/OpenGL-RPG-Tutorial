#pragma once
#include "texture.h"
#include "shader.h"

/*
* Abstract holding class for texture shader combinations used for rendering
* @author David Wadsworth
*/

class Material
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
