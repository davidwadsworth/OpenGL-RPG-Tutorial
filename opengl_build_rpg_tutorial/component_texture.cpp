#include "component_texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

void Component::Texture::load(const GLchar* tex_file_name)
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
	
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// Free image data
	stbi_image_free(image);
}

void Component::Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id_);
}
