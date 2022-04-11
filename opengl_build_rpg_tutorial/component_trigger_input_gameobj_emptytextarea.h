#pragma once
#include "component_rect_bitmap_glyph.h"
#include "component_system_render_draw.h"
#include "component_material_color.h"
#include "component_engine.h"
#include "component_system_render_empty.h"
#include "component_trigger_input.h"
#include "delimiter_split.h"
#include "component_json.h"

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace GameObj
			{
				class EmptyTextArea : public Component::Trigger::IInput
				{
					void create(Entity* gamestate) override
					{
						auto textarea_json = Game::global->get_child("index")->get_component<Component::Json>(name_)->json;

						std::string font_name = textarea_json["font"];
						float render_group = textarea_json["render_group"];
						int max_characters = textarea_json["max_characters"];

						auto& c_renderer = *gamestate->get_component<Component::Renderer>("renderer");
						auto& c_font_material = *gamestate->get_child(font_name)->get_component<Component::Material>("material");

						std::vector<Component::Transform*> transform_vec;
						std::vector<Component::Src*> src_vec;
						for (auto i = 0ull; i < max_characters; ++i)
						{
							src_vec.push_back(entity_->push_back_component<Component::Src>());
							transform_vec.push_back(entity_->push_back_component<Component::Transform>());
						}

						auto csi_empty = entity_->add_id_component<Component::System::Render::Empty>("render", src_vec, transform_vec, c_renderer, c_font_material);
						gamestate->get_component<Component::Engine>("render")->add(csi_empty, render_group);
					}
				
				public:
					using Component::Trigger::IInput::IInput;
				};
			}
		}
	}
}

