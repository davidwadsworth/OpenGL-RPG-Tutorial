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
				float box_h_, box_w_, corner_size_, scale_;
				std::string textbox_name_, box_name_, spritesheet_name_;
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

					auto offset_x = item_w / 2.0f - corner_size_ * 2.0f;
					pos_.x = item_x + offset_x;
					pos_.y = item_y + offset_y - box_h_;
					is_speech_arrow_ = json["message"]["speech_box"] == "true";
					textbox_name_ = json["textbox"];
				}

				void execute(Entity* gamestate) override
				{
					auto e_textarea = gamestate->get_child(textbox_name_);
					auto& csr_empty = *e_textarea->get_component<Component::System::Render::BlockDraw>("block_draw");

					box_w *= box_scale;
					box_h *= box_scale;

					float corner_size = box_json["corner_size"];
					float render_group = box_json["render_group"];

					auto scaled_corner_size = corner_size * box_scale;

					auto& c_renderer = *gamestate->get_component<Component::Renderer>("renderer");



					std::vector<Component::Transform*> transform_vec;
					// box corners transforms
					transform_vec.push_back(entity_->add_id_component<Component::Transform>("top_right",
						Rect(box_x, box_y, scaled_corner_size, scaled_corner_size)));
					transform_vec.push_back(entity_->add_id_component<Component::Transform>("top_left",
						Rect(box_x + box_w - scaled_corner_size, box_y, scaled_corner_size, scaled_corner_size)));
					transform_vec.push_back(entity_->add_id_component<Component::Transform>("bot_right",
						Rect(box_x, box_y + box_h - scaled_corner_size, scaled_corner_size, scaled_corner_size)));
					transform_vec.push_back(entity_->add_id_component<Component::Transform>("bot_left",
						Rect(box_x + box_w - scaled_corner_size, box_y + box_h - scaled_corner_size, scaled_corner_size, scaled_corner_size)));

					// box side transforms
					transform_vec.push_back(entity_->add_id_component<Component::Transform>("top_side",
						Rect(box_x + scaled_corner_size, box_y, box_w - scaled_corner_size * 2, scaled_corner_size)));
					transform_vec.push_back(entity_->add_id_component<Component::Transform>("left_side",
						Rect(box_x, box_y + scaled_corner_size, scaled_corner_size, box_h - scaled_corner_size * 2.0f)));
					transform_vec.push_back(entity_->add_id_component<Component::Transform>("right_side",
						Rect(box_x + box_w - scaled_corner_size, box_y + scaled_corner_size, scaled_corner_size, box_h - scaled_corner_size * 2.0f)));
					transform_vec.push_back(entity_->add_id_component<Component::Transform>("bot_side",
						Rect(box_x + scaled_corner_size, box_y + box_h - scaled_corner_size, box_w - scaled_corner_size * 2.0f, scaled_corner_size)));

					// center transform
					transform_vec.push_back(entity_->add_id_component<Component::Transform>("center",
						Rect(box_x + scaled_corner_size, box_y + scaled_corner_size, box_w - scaled_corner_size * 2.0f, box_h - scaled_corner_size * 2.0f)));

					// speech arrow
					transform_vec.push_back(entity_->add_id_component<Component::Transform>("speech_arrow", box_x + scaled_corner_size, box_y + box_h - scaled_corner_size, scaled_corner_size * 2.0f, scaled_corner_size));

					// get textbox shader and texture
					auto e_spritesheet = gamestate->get_child(spritesheet_name_);
					auto& c_ss_material = *e_spritesheet->get_component<Component::Material>("material");


					// get src rects
					std::vector<Component::Src*> box_srcs;
					for (std::string texture_id : box_json["texture_ids"])
						box_srcs.push_back(e_spritesheet->get_component<Component::Src>(texture_id));


					auto& render_engine = *gamestate->get_component<Component::Engine>("render");
					render_engine.add(cs_item, render_group);

					auto e_box = gamestate->get_child(textbox_name_)->get_child("box");

					auto& c_position = *e_box->get_component<Component::Position>("position");
					
					c_position.x = -pos_.x;
					c_position.y = -pos_.y;
					
					if (!is_speech_arrow_)
					{
						auto& c_speech_arrow_trans = *e_box->get_component<Component::Transform>("speech_arrow");
	
						speech_arrow_rect_.set(c_speech_arrow_trans);
						c_speech_arrow_trans.set(Game::removed.x, Game::removed.y, 0.0f, 0.0f);
					}
				}
			};
		}
	}
}