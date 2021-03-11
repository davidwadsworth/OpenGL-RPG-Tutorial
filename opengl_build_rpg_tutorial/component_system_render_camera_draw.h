#pragma once
#include "component_src.h"
#include "component_dest.h"
#include "component_transform.h"
#include "component_system.h"
#include "component_renderer.h"

/*
Draw sprites on screen with respect to where the camera is located

@author David Wadsworth
*/

namespace ComponentSystemRender
{
	class CameraDraw : public Component::System
	{
		Component::Renderer& renderer_;
		Component::Src& src_;
		Component::Dest& dest_;
		Component::Material& material_;
		Component::Transform& transform_, &camera_transform;
	public:
		CameraDraw(Component::Renderer& renderer, Component::Src& src, Component::Dest& dest, Component::Material& material,
			Component::Transform& transform, Component::Transform& camera_transform)
			: renderer_(renderer), src_(src), dest_(dest), material_(material), transform_(transform), camera_transform(camera_transform)
		{}

		void execute() override
		{
			// update render dest by camera and local transforms
			dest_.x = transform_.x - camera_transform.x;
			dest_.y = transform_.y - camera_transform.y;
			dest_.w = transform_.w * transform_.sc;
			dest_.h = transform_.h * transform_.sc;

			renderer_.draw(src_, dest_, material_);
		}
	};
}



