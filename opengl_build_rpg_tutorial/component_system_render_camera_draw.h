#pragma once
#include "component_rect.h"
#include "component_system.h"
#include "component_renderer.h"
#include "component_position.h"

/*
Draw sprites on screen with respect to where the camera is located

@author David Wadsworth
*/

namespace Component {
	namespace System {
		namespace Render 
		{
			class CameraDraw : public Component::ISystem
			{
				Component::Renderer& renderer_;
				Component::Src& src_;
				Component::Material& material_;
				Component::Transform& transform_;
				Component::Position& cam_position_;
			public:
				CameraDraw(Component::Renderer& renderer, Component::Src& src, Component::Transform& transform, 
					Component::Material& material, Component::Position& cam_position)
					: renderer_(renderer), src_(src), material_(material), transform_(transform), cam_position_(cam_position)
				{}

				void execute() override
				{
					// update render dest by camera and local transforms
					Rect dest;

					dest.x = transform_.x - cam_position_.x;
					dest.y = transform_.y - cam_position_.y;
					dest.w = transform_.w;
					dest.h = transform_.h;

					renderer_.draw(src_, dest, material_);
				}
			};
		}
	}
} 



