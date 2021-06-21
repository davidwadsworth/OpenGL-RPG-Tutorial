#include "component_material_color.h"

void Component::Material::Color::compile()
{
	Material::compile();
	shader.set_vec3f("color", color_);
}
