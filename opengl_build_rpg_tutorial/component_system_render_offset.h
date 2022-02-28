#pragma once
#include "component_system.h"
#include "component_renderer.h"
#include "component_rect.h"
#include "component_position.h"

namespace Component {
	namespace System {
		namespace Render 
		{
			class Offset : public Component::ISystem
			{
				Component::Position& offset_;
				std::vector<Component::Src*> srcs_;
				std::vector<Component::Transform*> transforms_;
				Component::Material& material_;
				Component::Renderer& renderer_;
			public:
				
				Offset(Component::Position& offset, std::vector<Component::Src*> srcs, std::vector<Component::Transform*> transforms,
				Component::Material& material, Component::Renderer& renderer)
					: offset_(offset), srcs_(srcs), transforms_(transforms), material_(material), renderer_(renderer)
				{}

				void execute() override
				{
					for (auto i = 0; i < srcs_.size(); ++i)
					{
						Rect dest;

						dest.x = transforms_[i]->x + offset_.x;
						dest.y = transforms_[i]->y + offset_.y;
						dest.w = transforms_[i]->w;
						dest.h = transforms_[i]->h;

						renderer_.draw(*srcs_[i], dest, material_);
					}
				}
			};
		}
	}
}