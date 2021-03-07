#pragma once
#include "component_render.h"
#include "component_transform.h"
#include "component_system.h"
#include "component_renderer.h"

namespace ComponentSystemRender
{
	class DynamicDraw : public Component::System
	{
		Component::Renderer& renderer_;
		Component::Render& render_;
		Component::Material& material_;
		Component::Transform& transform_, &camera_transform;
	public:
		DynamicDraw(Component::Renderer& renderer, Component::Render& render, Component::Material& material, 
			Component::Transform& transform, Component::Transform& camera_transform)
			: renderer_(renderer), render_(render), material_(material), transform_(transform), camera_transform(camera_transform)
		{}

		void execute() override
		{
			// update render dest by camera and local transforms
			render_.dest.x = transform_.rect.x - camera_transform.rect.x;
			render_.dest.y = transform_.rect.y - camera_transform.rect.y;
			render_.dest.w = transform_.rect.w * transform_.scale;
			render_.dest.h = transform_.rect.h * transform_.scale;

			renderer_.draw(render_, material_);
		}
	};
}



