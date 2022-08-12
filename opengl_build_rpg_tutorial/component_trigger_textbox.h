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
@author David Wadsworth
*/

namespace Component {
	namespace Trigger
	{
		class TextBox : public Component::ITrigger
		{
			nlohmann::json json_;

			void load_message(bool box, std::string message, Component::System::Update::Pathway::NavigatorTree* nav_tree)
			{
			}


			void load_content(nlohmann::json textbox_json, nlohmann::json pathway_json, Entity* gamestate, bool box, Component::System::Update::Pathway& pathway, NPATH* prev)
			{
				std::string name = "textbox";
				std::vector<ICommand*> commands;


				if (pathway_json.contains("pathway"))
					name = pathway_json["pathway"];


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

				bool has_box = textbox_json.contains("box");

				auto& cam_position = *gamestate->get_child("camera")->get_component<Component::Position>();

				combined_json["rect"] = json_["rect"];
				combined_json["rect"]["x"] = combined_json["rect"]["x"].get<float>() - cam_position.x;  
				combined_json["rect"]["y"] = combined_json["rect"]["y"].get<float>() - cam_position.y;

				combined_json["textbox"] = textbox_name;
				combined_json["message"] = msg_pos_json;

				auto textbox_obj = json_.get<nlohmann::json::object_t>();


				auto e_textbox = gamestate->get_child(textbox_name);

				auto ct_swap_vectors = e_textbox->get_component<Component::Trigger::SwapVectors>("swap_vectors");
				auto ct_clearempty = e_textbox->get_component<Component::Trigger::ClearBlockDraw>("clear_block_draw");
				auto& csu_pathway = *gamestate->get_component<Component::System::Update::Pathway>("pathway");

				load_content(combined_json, msg_pos_json, gamestate, )

			}
		};
	}
} 
