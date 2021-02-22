#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

Texture::Texture(GLuint internal_format, GLuint image_format, GLuint wrap_s, GLuint wrap_t, GLuint filter_min, GLuint filter_mag)
	: width(0), height(0), internal_format(internal_format), image_format(image_format), wrap_s(wrap_s), wrap_t(wrap_t), filter_min(filter_min),
	filter_mag(filter_mag)
{
	glGenTextures(1, &id_);
}

Texture::Texture(GLuint format)
	: Texture(format, format, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST)
{}

Texture::Texture()
	: Texture(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST)
{}

Texture::~Texture()
{
	glDeleteTextures(1, &id_);
}

void Texture::load(const GLchar* tex_file_name)
{
	// Load image
	int width, height, nr_channels;

	unsigned char* image = stbi_load(tex_file_name, &width, &height, &nr_channels, STBI_rgb_alpha);

	if (!image)
	{
		std::cout << "Failed to load texture: " << tex_file_name << std::endl;
		throw;
	}
	

	// set dimensions
	this->width = width;
	this->height = height;

	glBindTexture(GL_TEXTURE_2D, this->id_);

	// Create texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_mag);

	// Create Texture
	glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format, this->width, this->height, 0, this->image_format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// Free image data
	stbi_image_free(image);
}

void Texture::bind()
{
	// activate tex slot then bind the texture to active tex slot
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id_);
}
