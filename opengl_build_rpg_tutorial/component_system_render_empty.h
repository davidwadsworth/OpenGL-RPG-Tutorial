#pragma once
#include "component_system.h"
#include <vector>
#include "component_rect.h"
#include "component_renderer.h"
/*
Draw x src and transforms from one material, where x = draw_calls_.

@author David Wadsworth
*/
namespace Component {
	namespace System {
		namespace Render
		{
			class Empty : public Component::ISystem
			{
				std::size_t draw_calls_;
				std::vector<Component::Src*> srcs_;
				std::vector<Component::Transform*> transforms_;
				Component::Renderer& renderer_;
				Component::Material& material_;
			public:
				Empty(std::vector<Component::Src*> srcs, std::vector<Component::Transform*> transforms, Component::Renderer& renderer, Component::Material& material)
					: srcs_(srcs), transforms_(transforms), renderer_(renderer), material_(material), draw_calls_(0)
				{}

				void set_draw_calls(std::size_t draw_calls) { draw_calls_ = draw_calls; }

				void execute() override final
				{
					for (auto i = 0ull; i < draw_calls_; ++i)
					{
						renderer_.draw(*srcs_[i], *transforms_[i], material_);
					}
				}
			};
		}
	}
}

