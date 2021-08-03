#pragma once
#include "component_src.h"
#include "component_transform.h"
#include "component_system.h"
#include "component_renderer.h"

/*
Draw sprites on screen

@author David Wadsworth
*/

namespace Component {
	namespace System {
		namespace Render
		{
			class Draw : public Component::ISystem
			{
				Component::Renderer& renderer_;
				Component::Src& src_;
				Component::Material& material_;
				Component::Transform& transform_;
			public:
				Draw(Component::Renderer& renderer, Component::Src& src, Component::Transform& transform,
					Component::Material& material)
					: renderer_(renderer), src_(src), material_(material), transform_(transform)
				{}

				void execute() override
				{
					renderer_.draw(src_, transform_, material_);
				}
			};
		}
	}
}