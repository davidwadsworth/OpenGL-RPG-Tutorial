#pragma once
#include "component_vector.h"
#include "component_trigger_input.h"
#include "component_system.h"
#include <sstream>
#include "json.hpp"

/*
@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class TextBox : public Component::Trigger::IInput
			{
				std::string path_;
				glm::vec2 pos_;
			public:
				TextBox(std::string name, std::string path, glm::vec2 pos)
					: Component::Trigger::IInput(name), path_(path), pos_(pos)
				{}

			private:
				void create(Entity* gamestate) override final
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
					int box_h = msg_json["box_height"];
					int box_w = msg_json["box_width"];
					float box_sc = msg_json["box_scale"];
					std::string font = msg_json["font"];
					float font_sc = msg_json["font_scale"];
					std::string align_h = msg_json["align_horizontal"];
					std::string align_v = msg_json["align_vertical"];
					float line_spacing = msg_json["line_spacing"];
					float padding = msg_json["padding"];
					std::string message = msg_json["message"];

					// create boxes

					 


				}
			};
		}

	}
}