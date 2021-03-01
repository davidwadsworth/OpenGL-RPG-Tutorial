#include "material.h"

GLuint Material::id_gen = 0;

void Material::compile()
{
	// set up tex unit in shader program
	shader.use();
	shader.set_int("image", tex_unit_);
}

void Material::bind()
{
	// activate defined tex unit and bind appropriate texture onto it
	glActiveTexture(GL_TEXTURE0 + tex_unit_);
	texture.bind();
}
