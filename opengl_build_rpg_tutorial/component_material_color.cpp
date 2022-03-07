#include "component_material_color.h"

void Component::MaterialColor::compile()
{
	Material::compile();
	shader.set_vec3f("spriteColor", color_);
}
