#pragma once
#include "component_rect_bitmap_glyph.h"
#include "component_system_render_draw.h"
#include "component_material_color.h"
#include "component_engine.h"
#include "component_system_render_empty.h"

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace GameObj
			{
				class Empty : public Component::Trigger::IInput
				{
					std::size_t sz_, render_group_;
					std::string material_;
					void create(Entity* gamestate) override
					{
						auto& c_renderer = *gamestate->get_component<Component::Renderer>("renderer");
						auto& c_font_material = *gamestate->get_child(material_)->get_component<Component::Material>();

						std::vector<Component::ISystem*> temp_render_systems;
						for (auto i = 0ull; i < sz_; ++i)
						{
							auto& c_src = *entity_->push_back_component<Component::Src>();
							auto& c_transform = *entity_->push_back_component<Component::Transform>();
							auto csr_draw = entity_->push_back_component<Component::System::Render::Draw>(c_renderer, c_src, c_transform, c_font_material);
							temp_render_systems.push_back(csr_draw);
						}

						auto csi_empty = entity_->add_id_component<Component::System::Render::Empty>("render", temp_render_systems);
						gamestate->get_component<Component::Engine>("render")->add(csi_empty, render_group_);
						auto ct_change_text_area = entity_->add_id_component<Component::Trigger::Load::TextArea>("load");
					}
				
				public:
					Empty(std::string name, std::string material, std::size_t sz, std::size_t render_group)
						: Component::Trigger::IInput(name), material_(material), sz_(sz), render_group_(render_group)
					{}
				};
			}
		}
	}
}

