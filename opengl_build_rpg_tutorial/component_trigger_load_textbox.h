#pragma once
#include "component_trigger.h"
#include "component_tree.h"
#include <sstream>
#include "json.hpp"
#include "component_trigger_load_textarea.h"
#include "component_trigger_load_box.h"

namespace Component { 
	namespace Trigger {
		namespace Load 
		{
			class TextBox : public Component::ITrigger
			{
				std::string path_;
				Entity* textbox_;
			public:
				TextBox(Entity* textbox)
					: textbox_(textbox)
				{}

				void execute(Entity* gamestate) override
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
			};
		} 
	} 
}
