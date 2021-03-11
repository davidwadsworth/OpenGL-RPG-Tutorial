#pragma once
#include <glad/glad.h>
#include <string>
#include "component.h"

/*
Image handling class. Responsible for loading and binding textures to tex slots.

@author David Wadsworth
*/
namespace Component
{
	class Texture : public Comp
	{
		GLuint id_;
	public:
		// image dimensions
		GLuint width, height;

		// texture format
		GLuint internal_format;
		GLuint image_format;

		// texture configuration
		GLuint wrap_s;
		GLuint wrap_t;
		GLuint filter_min;
		GLuint filter_mag;

		Texture(GLuint internal_format, GLuint image_format, GLuint wrap_s, GLuint wrap_t, GLuint filter_min, GLuint filter_mag)
			: width(0), height(0), internal_format(internal_format), image_format(image_format), wrap_s(wrap_s), wrap_t(wrap_t), filter_min(filter_min),
			filter_mag(filter_mag)
		{
			glGenTextures(1, &id_);
		}

		Texture(GLuint format)
			: Texture(format, format, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST)
		{}

		Texture()
			: Texture(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST)
		{}

		~Texture()
		{
			glDeleteTextures(1, &id_);
		}

		void load(const GLchar* tex_file_name);
		void bind();
	};
}