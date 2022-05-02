#pragma once
#include "component_trigger_load.h"
#include "component_rect.h"

namespace Component {
	namespace Trigger {
		namespace Load
		{
			class Box : public Component::Trigger::ILoad
			{
				Rect speech_arrow_rect_;
				bool is_speech_arrow_;
				glm::vec2 pos_;
				std::string textbox_name_, box_name_;
			public:

				void load(nlohmann::json json)
				{
					float item_x = json["rect"]["x"];
					float item_y = json["rect"]["y"];
					float item_w = json["rect"]["w"];
					float offset_y = json["box"]["offset"];
					
					float box_h = json["box"]["height"];

					float box_corner_size = json["box"]["corner_size"];
					float box_scale = json["box"]["scale"];

					auto scaled_corner = box_corner_size * box_scale;

					auto offset_x = item_w / 2.0f - scaled_corner * 2.0f;
					pos_.x = item_x + offset_x;
					pos_.y = item_y + offset_y - box_h;
					is_speech_arrow_ = json["message"]["speech_box"] == "true";
					textbox_name_ = json["textbox"];
				}

				void execute(Entity* gamestate) override
				{

					nlohmann::json box_json = Game::global->get_child("index")->get_component<Component::Json>(box_info[0])->json;

					std::string spritesheet_name = box_json["texture"];

					auto& c_box_position = *entity_->add_id_component<Component::Position>("position", Game::removed);

					auto box_x = 0.0f;
					auto box_y = 0.0f;
					float box_w = box_json["width"];
					float box_h = box_json["height"];

					float box_scale = box_json["scale"];

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
					auto e_spritesheet = gamestate->get_child(spritesheet_name);
					auto& c_ss_material = *e_spritesheet->get_component<Component::Material>("material");


					// get src rects
					std::vector<Component::Src*> box_srcs;
					for (std::string texture_id : box_json["texture_ids"])
						box_srcs.push_back(e_spritesheet->get_component<Component::Src>(texture_id));

					auto cs_item = entity_->add_id_component<Component::System::Render::Offset>("render", c_box_position, box_srcs, transform_vec, c_ss_material, c_renderer);

					auto& render_engine = *gamestate->get_component<Component::Engine>("render");
					render_engine.add(cs_item, render_group);



					if (box_i_ > 1)
						Logger::error("load box called more than twice or load not called.", Logger::HIGH);

					auto e_box = gamestate->get_child(textbox_name_)->get_child("box");

					auto& c_position = *e_box->get_component<Component::Position>("position");
					if (box_i_)
					{
						c_position.x = Game::removed.x;
						c_position.y = Game::removed.y;
					}
					else
					{
						c_position.x = -pos_.x;
						c_position.y = -pos_.y;
					}
					
					if (!is_speech_arrow_)
					{
						auto& c_speech_arrow_trans = *e_box->get_component<Component::Transform>("speech_arrow");

						if (box_i_)
							c_speech_arrow_trans.set(speech_arrow_rect_);
						else
						{
							speech_arrow_rect_.set(c_speech_arrow_trans);
							c_speech_arrow_trans.set(Game::removed.x, Game::removed.y, 0.0f, 0.0f);
						}	
					}

					box_i_++;
				}
			};
		}
	}
}