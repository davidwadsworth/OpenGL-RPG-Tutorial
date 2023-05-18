#pragma once
#include <sstream>
#include <vector>
#include "json.hpp"
#include "command_message.h"
#include "command_box.h"
#include "command_clearblockdraw.h"
#include "component_pathway.h"
#include "component_position.h"
#include "command_switchstate.h"
#include "component_json.h"
#include "command.h"
#include "game.h"
#include "component_pathway.h"

/*
json
{
	"load" : {
		"transform" : [...],
		"data" : {
			"name" : "..."
			"data" : {}? []? ""? // this changes based on load is used
			"speech_box" : "True? False?"
			}
		},
	"textbox" : {
		"box" : {...},
		"textarea" : {...},
		"optionbox" : {...},
		"max_blocks" : 512,
		"render_group" : 4.0,
		"nav" : "textbox",
		"render_box" : "true"
	}
}

@author David Wadsworth
*/

constexpr auto SCREEN_PADDING = 5.0f;

namespace Command {
		class TextBox : public ICommand
		{
			nlohmann::json json_;
			Component::Transform* transform_;
		
			Component::Pathway::NavigatorTree* parse_json(nlohmann::json json, Component::Pathway& pathway, 
				Component::Pathway::NavigatorTree* last_tree)
			{
				auto json_copy = json;

				if (json["load"]["name"] == "messages")
				{
					std::vector<Component::Pathway::NavigatorTree*> nav_tree_vec;

					if (json["textbox"].find("box") != json["textbox"].end())
						last_tree->command_json.push_back(json_copy);

					json_copy["load"]["data"] = json["load"]["data"][0];

					last_tree->command_json.push_back(json_copy);

					nav_tree_vec.push_back(last_tree);

					for (auto i = 1; i < json["load"]["data"].size(); ++i)
					{
						auto navigator_tree = pathway.add_nav_tree(json["textbox"]["nav"]);

						if (json["textbox"].find("box") != json["textbox"].end())
							navigator_tree->command_json.push_back(json);

						json_copy["load"]["data"] = json["load"]["data"][i];

						navigator_tree->command_json.push_back(json_copy);
						nav_tree_vec.push_back(navigator_tree);
					}

					for (auto i = 0; i < nav_tree_vec.size() - 1; ++i)
						nav_tree_vec[i]->children.push_back(nav_tree_vec[i + 1]);

					return nav_tree_vec.back();
				}
				else if (json["load"]["name"] == "option_box")
				{
					if (json["load"]["data"].find("nav") != json["load"]["data"].end())
						pathway.change_nav(json["load"]["data"]["nav"], last_tree);


					last_tree->command_json.push_back(json_copy);

					for (auto option : json["load"]["data"])
					{
						auto option_nav_tree = pathway.add_nav_tree(json["textbox"]["nav"]);

						json_copy["load"]["data"] = option["data"];
						json_copy["load"]["name"] = option["name"];

						parse_json(json_copy, pathway, option_nav_tree);
						last_tree->children.push_back(option_nav_tree);
					}
				}
				else if (json["load"]["name"] == "series")
				{
					Component::System::Update::Pathway::NavigatorTree* option_nav_tree;
					for (auto data : json["load"]["data"])
					{
						option_nav_tree = pathway.add_nav_tree(json["textbox"]["nav"]);
						json_copy["load"]["name"] = data["name"];
						json_copy["load"]["data"] = data["data"];
						
						parse_json(json_copy, pathway, option_nav_tree);
						last_tree->children.push_back(option_nav_tree);
					}
					return option_nav_tree;
				}
				else if (json["load"]["name"] == "concurrent")
				{
					for (auto data : json["load"]["data"])
					{
						json_copy["load"]["name"] = data["name"];
						json_copy["load"]["data"] = data["data"];
  
						parse_json(json_copy, pathway, last_tree);
					}
				}
				else
				{
					last_tree->command_json.push_back(json_copy);
				}
				return last_tree;
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
				combined_json["textbox"] = textbox_json;
				combined_json["textbox"]["name"] = textbox_name;

				auto& cam_position = *gamestate->get_child("camera")->get_component<Component::Position>();

				combined_json["load"]["rect"] = json_["rect"];
				// need to change this to be in bounds of the camera
				auto rect_x = combined_json["load"]["rect"]["x"] = combined_json["rect"]["x"].get<float>() - cam_position.x;
				auto rect_y = combined_json["load"]["rect"]["y"] = combined_json["rect"]["y"].get<float>() - cam_position.y;
				auto rect_w = combined_json["rect"]["w"].get<float>();
				auto rect_h = combined_json["rect"]["h"].get<float>();
				auto x_diff = rect_x + rect_w - (cam_position.x + Game::width / 2.0f - SCREEN_PADDING);
				if (x_diff > 0)
					combined_json["rect"]["x"] = rect_x - x_diff;

				auto y_diff = rect_y + rect_h - (cam_position.y + Game::height / 2.0f - SCREEN_PADDING);
				if (y_diff > 0)
					combined_json["rect"]["y"] = rect_y - y_diff;

				combined_json["textbox"] = textbox_name;
				combined_json["message"] = msg_pos_json;

				auto textbox_obj = json_.get<nlohmann::json::object_t>();

				auto& pathway = *gamestate->get_component<Component::Pathway>("pathway");

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

				parse_json(combined_json, pathway, initial_nav_tree);

				nlohmann::json load_trigger_command;
				load_trigger_command["name"] = "load_trigger";
				load_trigger_command["data"] = nlohmann::json::parse(R"({"name" : "clear_block_draw"})");

				pathway.add_end_commands(message_json["name"], std::vector<nlohmann::json>{swap_vectors_command, load_trigger_command});

				pathway.add_next(message_json["name"]);
			}
		};
	}
} 
