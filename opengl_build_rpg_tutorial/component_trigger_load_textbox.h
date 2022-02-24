#pragma once
#include "component_trigger.h"
#include "component_tree.h"
#include <sstream>
#include "json.hpp"
#include "component_trigger_load_textarea.h"
#include "component_trigger_load_box.h"

/*

box json
{
	"box_rect" : [float, float, float, float] (x, y, w, h),
	"box_scale" : float,
	"corner_size" : float,
	"textbox": [string, int],
	"speech_arrow" : one of "right", "left", "none"
}

textarea json 
{
	"textarea_rect" : [float, float, float, float] (x, y, w, h), 
	"font" : string,
	"font_scale" : float,
	"align_horizontal" : one of "center", "right", "left",
	"align_vertical" : one of "center", "top", "bottom",
	"line_spacing" : float,
	"message" : string,
	"textbox" : [string, int]
}	

*/


namespace Component { 
	namespace Trigger {
		namespace Load 
		{
			class TextBox : public Component::Trigger::ILoad
			{
			public:
				void execute(Entity* gamestate) override
				{
					float pos_x = json_["position_x"];
					float pos_y = json_["position_y"];
					std::string textbox_name = json_["textbox"][0];
					int textbox_pos = json_["textbox"][1];
					nlohmann::json workflow = json_["workflow"];
					nlohmann::json textbox_json = gamestate->get_child("index")->
						get_component<Component::Json>(textbox_name)->json[textbox_pos];
					std::string box_name = textbox_json["box"];
					auto box_info = delimiter_split(box_name.c_str(), '_');

					nlohmann::json box_json = gamestate->get_child("index")->
						get_component<Component::Json>(box_info[0])->json[box_info[1]];
					
					std::string textarea_name = textbox_json["textarea"];
					auto textarea_info = delimiter_split(textarea_name.c_str(), '_');

					nlohmann::json textarea_json = gamestate->get_child("index")->
						get_component<Component::Json>(textarea_info[0])->json[textarea_info[1]];

					// get relavent message info
					float box_h = box_json["box_height"];
					float box_w = box_json["box_width"];
					float box_sc = box_json["box_scale"];
					std::string font_name = textarea_json["font"];
					float font_sc = textarea_json["font_scale"];
					std::string align_h = textarea_json["align_horizontal"];
					std::string align_v = textarea_json["align_vertical"];
					float line_spacing = textarea_json["line_spacing"];
					float msg_padding_x = textarea_json["msg_padding"][0];
					float msg_padding_y = textarea_json["msg_padding"][1];
					float corner_size = box_json["corner_size"];
					bool speech_box = json_["speech_box"] == "true";
				
					auto& c_cam_transform = *gamestate->get_child("camera")->get_component<Component::Transform>();

					// set up load data for moving box on screen and removing after done.
					std::stringstream ss_box_json;
				
					auto speech_arrow_alignment = pos_x - c_cam_transform.x + box_w >= Game::width;
					auto box_x = speech_arrow_alignment ? pos_x - c_cam_transform.x - box_w : pos_x - c_cam_transform.x;
					auto box_y = pos_y - c_cam_transform.y - box_h - 2 * corner_size * box_sc;

					auto speech_arrow_str = speech_box ? (speech_arrow_alignment ? "right" : "left") : "none";

					ss_box_json << "{\"box_rect\":[\"" << box_x << "\",\"" << box_y << "\",\"" << box_w << "\",\"" << box_h << "\"],";
					ss_box_json << "\"box_scale\":\"" << box_sc << "\",";
					ss_box_json << "\"corner_size\":\"" << corner_size << "\",";
					ss_box_json << "\"textbox\":[" << textbox_name << ",\"" << textbox_pos << "\",";
					ss_box_json << "\"speech_arrow\":\"" << speech_arrow_str << "\",";

					auto e_textbox = gamestate->get_child(textbox_name)->get_child(textbox_pos);
					auto e_box = e_textbox->get_child("box");
					auto e_txta = e_textbox->get_child("textarea");

					auto &c_trigger_tree = *e_textbox->get_component<Component::TriggerTree>();
					c_trigger_tree.destroy();

					auto box_json = nlohmann::json::parse(ss_box_json);
					auto ctl_init_box = e_box->get_child("game_info")->get_component<Component::Trigger::Load::Box>("load_box_0");
					ctl_init_box->load(box_json);

					std::string offscreen_rect = "[\"" + std::to_string(Game::removed.x) + "\",\"" 
						+ std::to_string(Game::removed.y) + "\",\"0\",\"0\"],";
					box_json["box_rect"] = nlohmann::json::parse(offscreen_rect);
					
					auto ctl_remove_box = e_box->get_child("game_info")->get_component<Component::Trigger::Load::Box>("load_box_1");
					ctl_remove_box->load(box_json);

					c_trigger_tree.add(std::vector<Component::ITrigger*>{ctl_remove_box});

					// set up load data for textarea
					std::string parser = workflow["parser"];

					if (parser == "message")
					{
						std::vector<std::string> messages = workflow["data"];

						for (auto i = messages.size() - 1; i >= 1; --i)
						{
							std::stringstream ss_txta_json;
							ss_txta_json << "{\"textarea_rect\":[\"" << box_x + corner_size * box_sc / 2.0f + msg_padding_x 
								<< "\",\"" << box_y + corner_size * box_sc / 2.0f + msg_padding_y << "\",\"" 
								<< box_w - corner_size * box_sc - 2.0f * msg_padding_x << "\",\"" 
								<< box_h - corner_size * box_sc - 2.0f * msg_padding_y << "\"],";
							ss_txta_json << "\"font\":\"" << font_name << "\",";
							ss_txta_json << "\"font_scale\":\"" << font_sc << "\",";
							ss_txta_json << "\"align_horizontal\":\"" << align_h << "\",";
							ss_txta_json << "\"align_vertical\":\"" << align_v << "\",";
							ss_txta_json << "\"message\":\"" << messages[i] << "\",";
							ss_txta_json << "\"textbox\":[\"" << textbox_name << "\",\"" << textbox_pos << "\"],";

							auto msg_json = nlohmann::json::parse(ss_txta_json);
							auto ctl_msg_txta = e_txta->get_child("game_info")->
								get_component<Component::Trigger::Load::TextArea>("load_textarea_" + std::to_string(i));
							ctl_msg_txta->load(msg_json);
							c_trigger_tree.add(std::vector<Component::ITrigger*>{ctl_msg_txta});
						}
						
					}
					else if (parser == "option_box")
					{
						//TODO
					}
					auto ctl_first_msg_txta = e_txta->get_child("game_info")->
						get_component<Component::Trigger::Load::TextArea>("load_textarea_0");

					auto& c_trigger_vector = *gamestate->get_child("engine")->get_component<Component::TriggerVector>("trigger");
					c_trigger_vector.push_back(ctl_init_box);
					c_trigger_vector.push_back(ctl_first_msg_txta);
				}
			};
		} 
	} 
}
