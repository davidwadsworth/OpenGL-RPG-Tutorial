#pragma once
#include "load.h"
#include "component_system_render_blockdraw.h"
#include "component_vector.h"

/*

@author David Wadsworth
*/

namespace Command {
	namespace Load {

		class TextBox : public ILoad
		{
			std::string textarea_name_;
			std::string box_name_;
			int max_message_;
			float render_group_;
			int max_blocks_;
			std::string name_;
		public:
			void load(nlohmann::json json) override final
			{
				name_ = json["name"];
				textarea_name_ = json["textarea"];
				box_name_ = json["box"];
				max_message_ = json["max_message"];

				render_group_ = json["render_group"];
				max_blocks_ = json["max_blocks"];
			}

			void execute(Entity* gamestate) override final
			{
				auto e_textbox = gamestate->add_id_child(name_);

				auto& textbox_json = Game::global->get_child("index")->get_component<Component::Json>(name_)->json;
					
				auto& c_renderer = *gamestate->get_component<Component::Renderer>("renderer");

				std::vector<Component::System::Render::BlockDraw::Block> block_vec;
				auto e_blocks = e_textbox->add_id_child("blocks");

				for (auto i = 0ull; i < max_blocks_; ++i)
				{
					Component::System::Render::BlockDraw::Block temp_block{};
					temp_block.src = e_blocks->push_back_component<Component::Src>();
					temp_block.transform = e_blocks->push_back_component<Component::Transform>();
					block_vec.push_back(temp_block);
				}

				auto csi_block_draw = e_textbox->add_id_component<Component::System::Render::BlockDraw>("render", block_vec, c_renderer);
				gamestate->get_component<Component::Engine>("render")->add(csi_block_draw, render_group_);
			}
		};
	}
}