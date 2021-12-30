#pragma once
#include "component_vector.h"
#include "component_trigger_input_game_obj.h"
#include "component_system.h"
#include <sstream>
#include <vector>
#include "json.hpp"
#include "component_trigger_delete_game_obj.h"
#include "component_trigger_delete_entity.h"
#include "component_trigger_switch_systems.h"
#include "component_trigger_input_game_obj_text_area.h"
#include "component_trigger_input_game_obj_box.h"
#include "component_quadly_linked_tree.h"
#include "component_system_update_traverse_tree.h"

/*
@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace GameObj
			{
				class TextBox : public Component::Trigger::Input::IGameObj
				{
					std::string path_;
					glm::vec2 pos_;
				public:
					TextBox(std::string name, std::string path, glm::vec2 pos)
						: Component::Trigger::Input::IGameObj(name), path_(path), pos_(pos)
					{}

				private:
					void init(Entity* gamestate) override final
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

						std::vector<Component::ITrigger*> add_triggers;

						auto ctigo_box = entity_->add_id_component<Component::Trigger::Input::GameObj::Box>("box", name_ + "_box",
							5, Rect{pos_.x, pos_.y, box_w, box_h}, corner_size, box_sc, speech_box);
						ctigo_box->execute(gamestate);

						add_triggers.push_back(gamestate->get_child(name_ + " box")->
							get_child("game info")->get_component<Component::Trigger::AddGameObj>());

						auto& c_trigger_qlt = *entity_->add_id_component<Component::QLTriggerTree>("quadly linked trigger tree");
						auto csu_tree_traverse = entity_->add_id_component<Component::System::Update::TraverseTree>("traverse tree");

						for (auto i = messages.size() - 1; i >= 0; --i)
						{
							auto ctigo_text_area = entity_->add_id_component<Component::Trigger::Input::GameObj::TextArea>(
								"text area " + std::to_string(i), name_ + " text area " + std::to_string(i), 5, font_name,
								messages[i], Rect{pos_.x + msg_padding_x, pos_.y + msg_padding_y, box_w - 2 * msg_padding_x,
								box_h - 2 * msg_padding_y}, line_spacing, font_sc, align_h, align_v);
							
							ctigo_text_area->execute(gamestate);
							
							auto e_txta_game_info = gamestate->get_child(name_ + " text area " + std::to_string(i))->get_child("game info");
							add_triggers.push_back(e_txta_game_info->get_component<Component::Trigger::AddGameObj>());
							c_trigger_qlt.push_child(e_txta_game_info->get_component<Component::Trigger::RemoveGameObj>());
						}
						
						auto& c_tbox_trigger_vector_add = *e_game_info_->add_id_component<Component::TriggerVector>("trigger add");
						auto& c_tbox_update_systems = *e_game_info_->add_id_component<Component::GroupedSystems>("update");

						c_tbox_trigger_vector_add.insert(c_tbox_trigger_vector_add.end(), add_triggers.begin(), add_triggers.end());

					}
				};
			}
		}
	}
}