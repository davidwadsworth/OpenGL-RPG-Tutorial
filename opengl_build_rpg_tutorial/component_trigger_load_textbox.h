#pragma once
#include "component_trigger.h"
#include "component_tree.h"
#include <sstream>
#include "json.hpp"
#include "component_trigger_load_message.h"
#include "component_trigger_load_box.h"
#include "component_trigger_clearempty.h"

/*
@author David Wadsworth
*/

namespace Component { 
	namespace Trigger {
		namespace Load 
		{
			class TextBox : public Component::Trigger::ILoad
			{
				nlohmann::json json_;
			public:
				void load(nlohmann::json json) override
				{
					json_ = json;
				}

				void execute(Entity* gamestate) override
				{
					std::string message_filename = json_["message"]["filename"];
					int message_pos = json_["message"]["pos"];
					auto message_json = gamestate->get_child("index")->get_component<Component::Json>(message_filename)->json;
					std::string textbox_name = message_json["textbox"];
					auto msg_pos_json = message_json["data"][message_pos];

					auto message_size = msg_pos_json["messages"].size();

					auto textbox_json = Game::global->get_child("index")->get_component<Component::Json>(textbox_name)->json;

					nlohmann::json combined_json;
					textbox_json["textbox"] = textbox_name;

					auto& cam_position = *gamestate->get_child("camera")->get_component<Component::Position>();

					combined_json["rect"] = json_["rect"];
					combined_json["rect"]["x"] = combined_json["rect"]["x"].get<float>() - cam_position.x;
					combined_json["rect"]["y"] = combined_json["rect"]["y"].get<float>() - cam_position.y;

					combined_json["textbox"] = textbox_name;
					combined_json["message"] = msg_pos_json;

					auto textbox_obj = json_.get<nlohmann::json::object_t>();


					auto e_textbox = gamestate->get_child(textbox_name);

					auto ct_swap_vectors = e_textbox->get_component<Component::Trigger::SwapVectors>("swap_vectors");
					auto& c_trigger_tree = *e_textbox->get_component<Component::TriggerTree>("trigger_tree");
					auto& csu_traverse_tree = *e_textbox->get_component<Component::System::Update::TraverseTree>("traverse_tree");
					auto ct_clearempty = e_textbox->get_component<Component::Trigger::ClearEmpty>("clear_empty");


					std::vector<Component::ITrigger*> middle_triggers{ct_clearempty};

					// move textbox into screen and pause game
					ct_swap_vectors->execute(gamestate);
					for (auto obj : textbox_obj)
					{
						std::string obj_index_path = obj.second;
						combined_json[obj.first] = Game::global->get_child("index")->get_component<Component::Json>(obj_index_path)->json;

						auto obj_load = gamestate->get_child("load")->get_component<Component::Trigger::ILoad>(obj_index_path);
						obj_load->execute(gamestate);
						middle_triggers.push_back(obj_load);
					}

					std::vector<Component::ITrigger*> end_triggers{ ct_swap_vectors, ct_clearempty };
					// add end triggers
					c_trigger_tree.add(end_triggers);

					// add message box triggers besides the first one.
					for (auto i = 0; i < message_size; ++i)
						c_trigger_tree.add(middle_triggers);
					
					csu_traverse_tree.set_cursor(c_trigger_tree);

				}
			};
		} 
	} 
}
