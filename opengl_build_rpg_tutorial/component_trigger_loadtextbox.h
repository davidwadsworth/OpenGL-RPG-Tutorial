#pragma once
#include "component_trigger.h"
#include <sstream>
#include "json.hpp"

namespace Component {
	namespace Trigger
	{
		class LoadTextBox : public Component::ITrigger
		{
			std::string path_;
		public:
			LoadTextBox()
			{}

			void change_path(std::string path)
			{}

			void execute(Entity* gamestate)
			{
				// load tilemap from file
				std::stringstream msg_stream;

				try
				{
					// open files
					std::ifstream msg_file(path_);

					// read into temp string streams
					msg_stream << msg_file.rdbuf();

					// close file streams
					msg_file.close();
				}
				catch (std::exception e)
				{
					Logger::error("Failed to read message file! path = " + path_, Logger::MEDIUM);
					return;
				}

				// parse into json obj
				auto msg_json = nlohmann::json::parse(msg_stream);

				// get relavent message info
				float box_h = msg_json["box_height"];
				float box_w = msg_json["box_width"];
				float box_sc = msg_json["box_scale"];
				std::string font_name = msg_json["font"];
				float font_sc = msg_json["font_scale"];
				std::string align_h = msg_json["align_horizontal"];
				std::string align_v = msg_json["align_vertical"];
				float line_spacing = msg_json["line_spacing"];
				float text_padding = msg_json["text_padding"];
				float msg_padding_x = msg_json["msg_padding_x"];
				float msg_padding_y = msg_json["msg_padding_y"];
				float corner_size = msg_json["corner_size"];
				bool speech_box = msg_json["speech_box"] == "true";
				std::vector<std::string> messages = msg_json["message"];
			}

			for (auto i = 0; i < messages.size(); ++i)
			{
				auto ctigo_text_area = entity_->add_id_component<Component::Trigger::Input::GameObj::TextArea>(
					"text_area_" + std::to_string(i), "text_area_" + std::to_string(i), entity_, render_group_, font_name,
					messages[i], Rect{ pos_.x + msg_padding_x, pos_.y + msg_padding_y, box_w - 2 * msg_padding_x,
					box_h - 2 * msg_padding_y }, line_spacing, font_sc, align_h, align_v);

				ctigo_text_area->execute(gamestate);
			}

			for (auto i = messages.size() - 1; i > 0; --i)
			{
				auto e_txt_curr_game_info = entity_->get_child("text_area_" + std::to_string(i))->get_child("game_info");
				auto e_txt_prev_game_info = entity_->get_child("text_area_" + std::to_string(i - 1))->get_child("game_info");
				std::vector<Component::ITrigger*> msg_triggers;

				c_trigger_tree.add(msg_triggers);
			}

			auto e_txt_end_game_info = entity_->get_child("text_area_" + std::to_string(messages.size() - 1));
			std::vector<Component::ITrigger*> msg_triggers;
			msg_triggers.push_back(e_txt_end_game_info->get_component<Component::Trigger::RemoveGameObj>("remove"));
			msg_triggers.push_back(e_box_game_info->get_component<Component::Trigger::RemoveGameObj>("remove"));
			msg_triggers.push_back(ct_pause_switch_systems);
			msg_triggers.push_back(entity_->add_id_component<Component::Trigger::DeleteEntity>("delete_entity", name_));
		};
	}
}