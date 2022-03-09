#pragma once
#include "component.h"
#include <glad/glad.h>
#include "logger.h"

constexpr auto MAX_TEX_UNITS = 32u;

/*
TODO

@author David Wadsworth
*/

namespace Component
{
	class TexUnit : public IComponent
	{
		GLuint open_tex_unit_;
	public:
		TexUnit()
			: open_tex_unit_(0u)
		{}

		void reset() 
		{
			open_tex_unit_ = 0u;
		}

		GLuint get_open_tex_unit()
		{
			if (open_tex_unit_ > MAX_TEX_UNITS)
				Logger::error("A lot of textures are being used, increment MAX_TEX_UNITS if this is normal", Logger::HIGH);

			return open_tex_unit_++;
		}
	};
}