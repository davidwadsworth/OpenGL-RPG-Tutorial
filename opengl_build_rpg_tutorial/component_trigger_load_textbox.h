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
	"corner_size" : int,
	"textbox": [string, int],
	"speech_arrow" : one of "right", "left", "none"
	"position" : [float, float],
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
			class TextBox : public Component::ITrigger
			{
			public:
				void execute(Entity* gamestate) override
				{
					
					

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
			};
		} 
	} 
}
