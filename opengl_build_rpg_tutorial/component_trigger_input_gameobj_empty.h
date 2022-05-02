#pragma once
#include "component_rect_bitmap_glyph.h"
#include "component_system_render_draw.h"
#include "component_material_color.h"
#include "component_engine.h"
#include "component_system_render_empty.h"
#include "component_trigger_input.h"
#include "delimiter_split.h"
#include "component_json.h"
#include "game.h"

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace GameObj
			{
				class Empty : public Component::Trigger::IInput
				{
					void create(Entity* gamestate) override
					{
						auto textarea_json = Game::global->get_child("index")->get_component<Component::Json>(name_)->json;

						float render_group = textarea_json["render_group"];
						int max_characters = textarea_json["max_slots"];

						auto& c_renderer = *gamestate->get_component<Component::Renderer>("renderer");

						std::vector<Component::System::Render::Empty::Block> block_vec;
						for (auto i = 0ull; i < max_characters; ++i)
						{
							Component::System::Render::Empty::Block temp_block{};
							temp_block.src = entity_->push_back_component<Component::Src>();
							temp_block.transform = entity_->push_back_component<Component::Transform>();
							block_vec.push_back(temp_block);
						}

						auto csi_empty = entity_->add_id_component<Component::System::Render::Empty>("render", block_vec, c_renderer);
						gamestate->get_component<Component::Engine>("render")->add(csi_empty, render_group);
					}
				
				public:
					using Component::Trigger::IInput::IInput;
				};
			}
		}
	}
}

