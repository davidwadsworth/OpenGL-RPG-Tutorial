#pragma once
#include "component_trigger.h"
#include <sstream>
#include <vector>
#include "json.hpp"
#include "load_message.h"
#include "load_box.h"
#include "component_trigger_clearblockdraw.h"
#include "component_system_update_pathway.h"
#include "component_position.h"
#include "component_trigger_swapvectors.h"
#include "component_json.h"
#include "game.h"

/*
message json

rect : [1, 1, 1, 1],
textarea : {...},
box : {...}
message : ""

has box should be in the textbox json file not if it has a box or not.

box json

rect : [...],
speech_box : "",
textbox : "",
box : {}

TODO finish optionbox before finishing this.


@author David Wadsworth
*/

namespace Component {
	namespace Trigger
	{
		class TextBox : public Component::ITrigger
		{
			nlohmann::json json_;
		

			Component::System::Update::Pathway::NavigatorTree* parse_json(nlohmann::json data_json,
				nlohmann::json textbox_json, Component::System::Update::Pathway& pathway, Component::System::Update::Pathway::NavigatorTree* last_tree)
			{
				if (data_json["name"] == "messages")
				{
					std::vector<Component::System::Update::Pathway::NavigatorTree*> nav_tree_vec;

					if (textbox_json.find("box") != textbox_json.end())
					{
						nlohmann::json box_command;
						box_command["name"] = textbox_json["box"];
						box_command["load"] = textbox_json; // this needs to be re thought because just the box data isn't enough
						last_tree->command_json.push_back(box_command);
					}

					nlohmann::json message_command;
					message_command["name"] = data_json["name"];
					message_command["load"] = data_json["data"][0];

					last_tree->command_json.push_back(message_command);

					nav_tree_vec.push_back(last_tree);

					for (auto i = 1; i < data_json["data"].size(); ++i)
					{
						auto navigator_tree = pathway.add_nav_tree(textbox_json["nav"]);

						if (textbox_json.find("box") != textbox_json.end())
						{
							nlohmann::json box_command;
							box_command["name"] = textbox_json["box"];
							box_command["load"] = textbox_json["box_data"];
							navigator_tree->command_json.push_back(box_command);
						}

						nlohmann::json message_command;
						message_command["name"] = data_json["name"];
						message_command["load"] = data_json["data"][i];

						navigator_tree->command_json.push_back(message_command);
						nav_tree_vec.push_back(navigator_tree);
					}

					for (auto i = 0; i < nav_tree_vec.size() - 1; ++i)
						nav_tree_vec[i]->children.push_back(nav_tree_vec[i + 1]);

					return nav_tree_vec.back();
				}
				else if (data_json["name"] == "option_box")
				{
					if (data_json.find("nav") != data_json.end())
						pathway.change_nav(data_json["nav"], last_tree);

					nlohmann::json option_box_command;
					option_box_command["name"] = "option_box";
					option_box_command["data"] = data_json["options"];

					last_tree->command_json.push_back(option_box_command);

					for (auto option : data_json["data"])
					{
						auto option_nav_tree = pathway.add_nav_tree(textbox_json["nav"]);
						parse_json(option, textbox_json, pathway, option_nav_tree);
						last_tree->children.push_back(option_nav_tree);
					}

					return last_tree;
				}
				else if (data_json["name"] == "series")
				{
					for (auto data : data_json["data"])
					{
						auto option_nav_tree = pathway.add_nav_tree(textbox_json["nav"]);
						parse_json(data, textbox_json, pathway, option_nav_tree);
						last_tree->children.push_back(option_nav_tree);
					}
				}
				else if (data_json["name"] == "concurrent")
				{
					for (auto data : data_json["data"])
						parse_json(data, textbox_json, pathway, last_tree);
				}
				else
				{
					nlohmann::json command;
					command["name"] = data_json["name"];
					command["load"] = data_json["data"];

					last_tree->command_json.push_back(command);
					return last_tree;
				}
			}
		
		public:

			TextBox(nlohmann::json json)
				: json_(json)
			{}

			void execute(Entity* gamestate) override
			{
				std::string message_filename = json_["message"]["filename"];
				int message_pos = json_["message"]["pos"];
				auto message_json = gamestate->get_child("index")->get_component<Component::Json>(message_filename)->json;
				std::string textbox_name = message_json["textbox"];
				auto msg_pos_json = message_json["data"][message_pos];

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

				auto& pathway = *gamestate->get_component<Component::System::Update::Pathway>("pathway");

				if (pathway.is_registered(message_json["name"]))
				{
					pathway.add_next(message_json["name"]);
					return;
				}

				// this needs to create the initial nav tree and then pass through a json string with the name textbox json and the json
				auto initial_nav_tree = pathway.add_nav_tree(combined_json["nav"]);

				pathway.register_nav_tree(message_json["name"], initial_nav_tree);

				nlohmann::json swap_vectors_command;
				swap_vectors_command["name"] = "load_trigger";
				swap_vectors_command["data"] = nlohmann::json::parse(R"({"name" : "swap_vectors"})");

				initial_nav_tree->command_json.push_back(swap_vectors_command);

				auto last_nav_tree = parse_json(message_json["data"], textbox_json, pathway, initial_nav_tree);

				nlohmann::json load_trigger_command;
				load_trigger_command["name"] = "load_trigger";
				load_trigger_command["data"] = nlohmann::json::parse(R"({"name" : "clear_block_draw"})");

				last_nav_tree->command_json.push_back(load_trigger_command);

				pathway.add_next(message_json["name"]);
			}
		};
	}
} 
