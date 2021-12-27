#pragma once
#include "component_vector.h"
#include "component_trigger_input.h"
#include "component_system.h"
#include <sstream>
#include <vector>
#include "json.hpp"
#include "component_template.h"
#include "component_tree.h"
#include "component_material.h"
#include "component_system_render_camera_draw.h"
#include "component_controller_keyboard.h"
#include "component_src_bitmap_glyph.h"
#include "component_trigger_delete_game_obj.h"
#include "component_trigger_add_game_obj.h"
#include "component_trigger_delete_entity.h"
#include "component_trigger_switch_systems.h"
#include "component_trigger_input_game_obj_text_area.h"

/*
@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace GameObj
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
						auto& cti_observer = *gamestate->get_component<Component::Trigger::Input::SystemObs>();

						auto e_observer = gamestate->get_child("observer");
						auto& c_tb_observer = *e_observer->add_id_component<Component::SystemObserver>("textbox");

						auto e_tb_reference = new Entity();
						entity_->add_id_child(e_tb_reference, "reference");
						auto& c_tb_ref_render_list = *e_tb_reference->add_id_component<Component::SystemList>("render systems");


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
						std::vector<std::string> message = msg_json["message"];

						
					}
				};
			}
		}
	}
}