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
					nlohmann::json textbox_json = gamestate->get_child("index")->get_child(textbox_name)->
						get_component<Component::Json>()->json[textbox_pos];

					// get relavent message info
					float box_h = textbox_json["box_height"];
					float box_w = textbox_json["box_width"];
					float box_sc = textbox_json["box_scale"];
					std::string font_name = textbox_json["font"];
					float font_sc = textbox_json["font_scale"];
					std::string align_h = textbox_json["align_horizontal"];
					std::string align_v = textbox_json["align_vertical"];
					float line_spacing = textbox_json["line_spacing"];
					float text_padding = textbox_json["text_padding"];
					float msg_padding_x = textbox_json["msg_padding_x"];
					float msg_padding_y = textbox_json["msg_padding_y"];
					float corner_size = textbox_json["corner_size"];
					bool speech_box = textbox_json["speech_box"] == "true";
				
					auto& c_cam_transform = *gamestate->get_child("camera")->get_component<Component::Transform>();

					// set up load data for moving box on screen and removing after done.
					std::stringstream ss_box_json;
				
					auto speech_arrow_alignment = pos_x - c_cam_transform.x + box_w >= Game::width;
					auto box_x = speech_arrow_alignment ? pos_x - c_cam_transform.x - box_w : pos_x - c_cam_transform.x;
					auto box_y = pos_y - c_cam_transform.y - box_h - 2 * corner_size * box_sc;

					auto speech_arrow_str = speech_box ? (speech_arrow_alignment ? "right" : "left") : "none";

					ss_box_json << "{\"box_rect\":[\"" << box_x << "\",\"" << box_y << "\",\"" << box_h << "\",\"" << box_w << "\"],";
					ss_box_json << "\"box_scale\":\"" << box_sc << "\",";
					ss_box_json << "\"corner_size\":\"" << corner_size << "\",";
					ss_box_json << "\"textbox\":[" << textbox_name << ",\"" << textbox_pos << "\",";
					ss_box_json << "\"speech_arrow\":\"" << speech_arrow_str << "\",";

					auto e_textbox = gamestate->get_child(textbox_name)->get_child(textbox_pos);
					auto e_box = e_textbox->get_child("box");

					auto box_json = nlohmann::json::parse(ss_box_json);
					e_box->get_child("game_info")->get_component<Component::Trigger::Load::Box>("load_box_0")->load(box_json);

					std::string offscreen_rect = "[\"" + std::to_string(Game::removed.x) + "\",\"" + std::to_string(Game::removed.y) + "\",\"0\",\"0\"],";
					box_json["box_rect"] = nlohmann::json::parse(offscreen_rect);
					
					e_box->get_child("game_info")->get_component<Component::Trigger::Load::Box>("load_box_1")->load(box_json);

					// set up load data for textarea
					std::string parser = workflow["parser"];

					if (parser == "message")
					{
						std::vector<std::string> messages = workflow["data"];

						for (auto i = 0; i < messages.size(); ++i)
						{
							std::stringstream ss_txta_json;
							ss_txta_json << "{\"textarea_rect\":[\"" << box_x + corner_size * box_sc / 2.0f + text_padding << "\",\"" << box_y + corner_size * box_sc / 2.0f + text_padding << "\",\"" 
								<< box_h - corner_size * box_sc - 2.0f * text_padding << "\",\"" << box_w - corner_size * box_sc - 2.0f * text_padding << "\"],";
							ss_txta_json << "\"font\":\"" << font_name << "\",";
							ss_txta_json << "\"font_scale\":\"" << font_sc << "\",";
							ss_txta_json << "\"align_horizontal\":\"" << align_h << "\",";
							ss_txta_json << "\"align_vertical\":\"" << align_v << "\",";
							ss_txta_json << "\"message\":\"" << messages[i] << "\",";
							ss_txta_json << "\"textbox\":[\"" << textbox_name << "\",\"" << textbox_pos << "\"],";

							auto msg_json = nlohmann::json::parse(ss_txta_json);

						}
					}
					else if (parser == "option_box")
					{
						//TODO
					}
				}
			};
		} 
	} 
}
