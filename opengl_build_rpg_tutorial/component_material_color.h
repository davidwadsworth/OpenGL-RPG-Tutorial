#pragma once
#include "component_material.h"

namespace Component{
	namespace Material {
		class Color : public Material
		{
			glm::vec3 color_;
		public:
			Color(Component::Texture& c_texture, Component::Shader& c_shader, GLuint tex_unit, glm::vec3 color)
				: Material(c_texture, c_shader, tex_unit), color_(color)
			{}

			void compile() override;
		};
	}
}
