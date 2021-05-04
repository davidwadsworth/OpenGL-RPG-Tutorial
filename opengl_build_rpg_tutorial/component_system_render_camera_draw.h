#pragma once
#include "component_src.h"
#include "component_transform.h"
#include "component_system.h"
#include "component_renderer.h"

/*
Draw sprites on screen with respect to where the camera is located

@author David Wadsworth
*/

namespace Component {
	namespace System {
		namespace Render 
		{
			class CameraDraw : public Component::Sys
			{
				Component::Renderer& renderer_;
				Component::Src& src_;
				Component::Material& material_;
				Component::Transform& transform_, & camera_transform;
			public:
				CameraDraw(Component::Renderer& renderer, Component::Src& src, Component::Transform& transform, 
					Component::Material& material, Component::Transform& camera_transform)
					: renderer_(renderer), src_(src), material_(material), transform_(transform), camera_transform(camera_transform)
				{}

				void execute() override
				{
					// update render dest by camera and local transforms
					Rect dest;

					dest.x = transform_.x - camera_transform.x;
					dest.y = transform_.y - camera_transform.y;
					dest.w = transform_.w * transform_.sc;
					dest.h = transform_.h * transform_.sc;

					renderer_.draw(src_, dest, material_);
				}
			};
		}
	}
} 



