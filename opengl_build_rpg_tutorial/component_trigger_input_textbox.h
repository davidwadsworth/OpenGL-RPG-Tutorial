#pragma once
#include "component_system_render_blockdraw.h"
#include "component_trigger_input_gameobj_box.h"
#include "component_vector.h"
#include "component_trigger_swapvectors.h"
#include "component_trigger_load_message.h"
#include "component_trigger_load_box.h"
#include "component_trigger_load_textbox.h"
#include "component_trigger_load_optionbox.h"

/*
@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			class TextBox : public Component::Trigger::IInput
			{
			private:
				void create(Entity* gamestate) override final
				{
					auto& textbox_json = Game::global->get_child("index")->get_component<Component::Json>(name_)->json;

					auto& c_update_vec = *entity_->add_id_component<Component::SystemVector>("update");
					auto ct_switch_vectors = entity_->add_id_component<Component::Trigger::SwapVectors>("swap_vectors", c_update_vec);

					// add message and box triggers to gameinfo
					std::string textarea_name = textbox_json["textarea"];
					std::string box_name = textbox_json["box"];
					int max_message = textbox_json["max_message"];

					float render_group = textbox_json["render_group"];
					int max_blocks = textbox_json["max_blocks"];

					auto& c_renderer = *gamestate->get_component<Component::Renderer>("renderer");

					std::vector<Component::System::Render::BlockDraw::Block> block_vec;
					auto e_blocks = entity_->add_id_child("blocks");

					for (auto i = 0ull; i < max_blocks; ++i)
					{
						Component::System::Render::BlockDraw::Block temp_block{};
						temp_block.src = e_blocks->push_back_component<Component::Src>();
						temp_block.transform = e_blocks->push_back_component<Component::Transform>();
						block_vec.push_back(temp_block);
					}

					auto csi_block_draw = entity_->add_id_component<Component::System::Render::BlockDraw>("render", block_vec, c_renderer);
					gamestate->get_component<Component::Engine>("render")->add(csi_block_draw, render_group);
						
					auto e_lc = gamestate->get_child("load");
					auto e_textbox = e_lc->add_id_child(name_);
					e_textbox->add_id_component<Component::Trigger::Load::TextBox>(name_);
					e_textbox->add_id_component<Component::Trigger::Load::Message>(textarea_name);
					e_textbox->add_id_component<Component::Trigger::Load::Box>(box_name);

					if (textbox_json.contains("optionbox"))
					{
						std::string optionbox_name = textbox_json["optionbox"];
						e_textbox->add_id_component<Component::Trigger::Load::OptionBox>(optionbox_name);
					}
				}
			public:
				using Component::Trigger::IInput::IInput;
			};
		}
	}
}