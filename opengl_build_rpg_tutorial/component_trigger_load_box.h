#pragma once
#include "component_trigger_load.h"
#include "component_rect.h"
#include "component_system_render_blockdraw.h"

namespace Component {
	namespace Trigger {
		namespace Load
		{
			class Box : public Component::Trigger::ILoad
			{
				bool is_speech_arrow_;
				glm::vec2 pos_;
				float box_x_, box_y_, box_h_, box_w_, corner_size_, scale_;
				std::string textbox_name_, box_name_, spritesheet_name_;
				std::vector<std::string> texture_ids_;
			public:

				void load(nlohmann::json json)
				{
					float item_x = json["rect"]["x"];
					float item_y = json["rect"]["y"];
					float item_w = json["rect"]["w"];
					float offset_y = json["box"]["offset"];

					auto scale = json["box"]["scale"];

					box_h_ = json["box"]["height"];
					box_w_ = json["box"]["width"];

					box_h_ *= scale;
					box_w_ *= scale;

					corner_size_ = json["box"]["corner_size"];
					corner_size_ *= scale;

					spritesheet_name_ = json["box"]["texture"];

					texture_ids_ = json["box"]["textures"].get<std::vector<std::string>>();

					auto offset_x = item_w / 2.0f - corner_size_ * 2.0f;
					box_x_ = item_x + offset_x;
					box_y_ = item_y + offset_y - box_h_;
					is_speech_arrow_ = json["message"]["speech_box"] == "true";
					textbox_name_ = json["textbox"];
				}

				void execute(Entity* gamestate) override
				{
					auto e_textarea = gamestate->get_child(textbox_name_);
					auto& csr_empty = *e_textarea->get_component<Component::System::Render::BlockDraw>("block_draw");

					auto& c_renderer = *gamestate->get_component<Component::Renderer>("renderer");

					auto blocks = csr_empty.get_blocks(10);

					// box corners transforms
					blocks[0].transform->set( Rect(box_x_, box_y_, corner_size_, corner_size_));
					blocks[1].transform->set(Rect(box_x_ + box_w_ - corner_size_, box_y_, corner_size_, corner_size_));
					blocks[2].transform->set(Rect(box_x_, box_y_ + box_h_ - corner_size_, corner_size_, corner_size_));
					blocks[3].transform->set(Rect(box_x_ + box_w_ - corner_size_, box_y_ + box_h_ - corner_size_, corner_size_, corner_size_));

					// box side transforms
					blocks[4].transform->set(Rect(box_x_ + corner_size_, box_y_, box_w_ - corner_size_ * 2, corner_size_));
					blocks[5].transform->set(Rect(box_x_, box_y_ + corner_size_, corner_size_, box_h_ - corner_size_ * 2.0f));
					blocks[6].transform->set(Rect(box_x_ + box_w_ - corner_size_, box_y_ + corner_size_, corner_size_, box_h_ - corner_size_ * 2.0f));
					blocks[7].transform->set(Rect(box_x_ + corner_size_, box_y_ + box_h_ - corner_size_, box_w_ - corner_size_ * 2.0f, corner_size_));

					// center transform
					blocks[8].transform->set(Rect(box_x_ + corner_size_, box_y_ + corner_size_, box_w_ - corner_size_ * 2.0f, box_h_ - corner_size_ * 2.0f));

					// speech arrow
					
					if (is_speech_arrow_)
						blocks[9].transform->set(Rect(box_x_ + corner_size_, box_y_ + box_h_ - corner_size_, corner_size_ * 2.0f, corner_size_));
					else
						blocks[9].transform->set(Game::removed.x, Game::removed.y, 0.0f, 0.0f);
					
					// get textbox shader and texture
					auto e_spritesheet = gamestate->get_child(spritesheet_name_);
					auto& c_ss_material = *e_spritesheet->get_component<Component::Material>("material");


					// get src rects
					std::vector<Component::Src*> box_srcs;
					auto i = 0;
					for (std::string texture_id : texture_ids_)
						blocks[i++].src->set(*e_spritesheet->get_component<Component::Src>(texture_id));					
					
				}
			};
		}
	}
}