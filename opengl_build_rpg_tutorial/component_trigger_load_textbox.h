#pragma once
#include "component_trigger.h"
#include "component_tree.h"
#include <sstream>
#include "json.hpp"
#include "component_trigger_load_message.h"
#include "component_trigger_load_box.h"

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
					std::string box_name = textbox_json["box"];
					auto box_json = Game::global->get_child("index")->get_component<Component::Json>(box_name)->json;
					std::string textarea_name = textbox_json["textarea"];
					auto textarea_json = Game::global->get_child("index")->get_component<Component::Json>(textarea_name)->json;

					nlohmann::json combined_json;
					textbox_json["textbox"] = textbox_name;

					auto& cam_position = *gamestate->get_child("camera")->get_component<Component::Position>();

					combined_json["rect"] = json_["rect"];
					combined_json["rect"]["x"] = combined_json["rect"]["x"].get<float>() - cam_position.x;
					combined_json["rect"]["y"] = combined_json["rect"]["y"].get<float>() - cam_position.y;

					combined_json["message"] = msg_pos_json;
					combined_json["box"] = box_json;
					combined_json["textarea"] = textarea_json;
					combined_json["textbox"] = textbox_name;

					auto box_load = gamestate->get_child("load")->get_component<Component::Trigger::Load::Box>(box_name);
					box_load->load(combined_json);

					auto textarea_load = gamestate->get_child("load")->get_component<Component::Trigger::Load::Message>(textarea_name);
					textarea_load->load(combined_json);
					
					auto e_textbox = gamestate->get_child(textbox_name);

					auto ct_swap_vectors = e_textbox->get_component<Component::Trigger::SwapVectors>("swap_vectors");
					auto& c_trigger_tree = *e_textbox->get_component<Component::TriggerTree>("trigger_tree");
					auto& csu_traverse_tree = *e_textbox->get_component<Component::System::Update::TraverseTree>("traverse_tree");

					// add end triggers
					c_trigger_tree.add(std::vector<Component::ITrigger*> {ct_swap_vectors, box_load, textarea_load});

					// add message box triggers besides the first one.
					for (auto i = message_size - 1; i > 0; --i)
						c_trigger_tree.add(std::vector<Component::ITrigger*>{textarea_load});
					
					csu_traverse_tree.set_cursor(c_trigger_tree);

					// move textbox into screen and pause game
					ct_swap_vectors->execute(gamestate);
					box_load->execute(gamestate);
					textarea_load->execute(gamestate);
				}
			};
		} 
	} 
}
