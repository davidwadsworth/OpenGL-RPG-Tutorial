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
#include "component_tree.h"
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
					std::size_t render_group_;
					glm::vec2 pos_;
				public:
					TextBox(std::string name, std::size_t render_group, std::string path, glm::vec2 pos)
						: Component::Trigger::Input::IGameObj(name), path_(path), render_group_(render_group), pos_(pos)
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

						auto& c_tbox_update_systems = *e_game_info_->add_id_component<Component::GroupedSystems>("update");
						auto& c_tbox_trigger_vector_add = *e_game_info_->add_id_component<Component::TriggerVector>("trigger_add");
						auto& c_tbox_trigger_vector_remove = *e_game_info_->add_id_component<Component::TriggerVector>("trigger_remove");

						auto e_pause = new Entity();
						entity_->add_id_child(e_pause, "pause");
						e_pause->add_component<Component::GroupedSystems>("update");


						auto ctigo_box = entity_->add_id_component<Component::Trigger::Input::GameObj::Box>("box", "box",
							entity_, render_group_, Rect{pos_.x, pos_.y, box_w, box_h}, corner_size, box_sc, speech_box);
						ctigo_box->execute(gamestate);

						auto e_box_game_info = gamestate->get_child("box")->get_child("game info");

						c_tbox_trigger_vector_add.push_back(e_box_game_info->get_component<Component::Trigger::AddGameObj>());
						c_tbox_trigger_vector_remove.push_back(e_box_game_info->get_component<Component::Trigger::RemoveGameObj>());

						auto& c_trigger_tree = *entity_->add_id_component<Component::TriggerTree>("trigger_tree");
						auto csu_tree_traverse = entity_->add_id_component<Component::System::Update::TraverseTree>("traverse_tree");

						for (auto i = 0; i < messages.size(); ++i)
						{
							auto ctigo_text_area = entity_->add_id_component<Component::Trigger::Input::GameObj::TextArea>(
								"text_area_" + std::to_string(i),"text_area_" + std::to_string(i), entity_, render_group_, font_name,
								messages[i], Rect{pos_.x + msg_padding_x, pos_.y + msg_padding_y, box_w - 2 * msg_padding_x,
								box_h - 2 * msg_padding_y}, line_spacing, font_sc, align_h, align_v);
							
							ctigo_text_area->execute(gamestate);
						}

						for (auto i = messages.size() - 1; i > 0; --i)
						{
							auto e_txt_curr_game_info = entity_->get_child("text_area_" + std::to_string(i))->get_child("game_info");
							auto e_txt_prev_game_info = entity_->get_child("text_area_" + std::to_string(i - 1))->get_child("game_info");
							std::vector<Component::ITrigger*> msg_triggers;
							
							msg_triggers.push_back(e_txt_prev_game_info->get_component<Component::Trigger::RemoveGameObj>("remove"));
							msg_triggers.push_back(e_txt_curr_game_info->get_component<Component::Trigger::AddGameObj>("add"));
							c_trigger_tree.add(msg_triggers);
						}

						auto e_txt_end_game_info = entity_->get_child("text_area_" + std::to_string(messages.size() - 1));
						std::vector<Component::ITrigger*> msg_triggers;
						msg_triggers.push_back(e_txt_end_game_info->get_component<Component::Trigger::RemoveGameObj>("remove"));
						msg_triggers.push_back(entity_->add_id_component<Component::Trigger::DeleteEntity>("delete_entity", name_));
						msg_triggers.push_back();

						auto e_txt_0_game_info = entity_->get_child("text_area_0")->get_child("game_info");

						c_tbox_trigger_vector_add.push_back(e_txt_0_game_info->get_component<Component::Trigger::AddGameObj>());


						
					}
				};
			}
		}
	}
}