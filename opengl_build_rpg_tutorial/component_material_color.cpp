#include "component_material_color.h"

void Component::Color::compile()
{
	Material::compile();
	shader.set_vec3f("spriteColor", color_);
}
