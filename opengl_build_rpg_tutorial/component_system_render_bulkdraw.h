#pragma once
#pragma once
#include "component_system.h"
#include "component_renderer.h"
#include "component_rect.h"
#include "component_position.h"

namespace Component {
	namespace System {
		namespace Render
		{
			class BulkDraw : public Component::ISystem
			{
				std::vector<Component::Src*> srcs_;
				std::vector<Component::Transform*> transforms_;
				Component::Material& material_;
				Component::Renderer& renderer_;
			public:

				BulkDraw(std::vector<Component::Src*> srcs, std::vector<Component::Transform*> transforms,
					Component::Material& material, Component::Renderer& renderer)
					: srcs_(srcs), transforms_(transforms), material_(material), renderer_(renderer)
				{}

				void execute() override
				{
					for (auto i = 0; i < srcs_.size(); ++i)
						renderer_.draw(*srcs_[i], *transforms_[i], material_);
				}
			};
		}
	}
}