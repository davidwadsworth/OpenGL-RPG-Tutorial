#pragma once
#include "component_trigger_input_gameobj_empty.h"
#include "component_trigger_input_gameobj_box.h"
#include "component_vector.h"
#include "component_trigger_swapengine.h"
#include "component_tree.h"
#include "component_system_update_traverse_tree.h"
#include "component_trigger_load_textarea.h"
#include "component_trigger_load_box.h"
#include "component_trigger_load_textbox.h"

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
				private:
					void init(Entity* gamestate) override final
					{
						auto textbox_info = delimiter_split(name_.c_str(), '_');

						if (textbox_info.size() < 2)
							Logger::error("textbox name not of name_pos", Logger::SEVERITY::HIGH);

						auto& textbox_json = gamestate->get_child("index")->get_child(textbox_info[0])->
							get_component<Component::Json>()->json[std::stoi(textbox_info[1])];

						int max_characters = textbox_json["max_characters"];
						std::string font_name = textbox_json["font"];
						float render_group = textbox_json["render_group"];
						float update_group = textbox_json["update_group"];

						auto e_font = gamestate->get_child(font_name);

						auto e_pause = new Entity();
						entity_->add_id_child(e_pause, "pause");
						auto& c_pause_update_engine = *e_pause->add_id_component<Component::Engine>("update");
						auto ct_pause_switch_systems = e_pause->add_id_component<Component::Trigger::SwapEngine>("switch_systems", c_pause_update_engine);

						auto ctigo_box = entity_->add_id_ct_input<Component::Trigger::Input::GameObj::Box>("box", render_group);
						ctigo_box->set_parent(entity_);
						ctigo_box->execute(gamestate);

						auto ctigo_empty = entity_->add_id_ct_input<Component::Trigger::Input::GameObj::Empty>("empty", e_font, max_characters, render_group);
						ctigo_empty->set_parent(entity_);
						ctigo_empty->execute(gamestate);
						
						// add message and box triggers to gameinfo
						int max_messages = textbox_json["max_message"];
						int max_boxes = textbox_json["max_box"];

						for (auto i = 0; i < max_messages; ++i)
							ctigo_empty->e_game_info_->add_id_component<Component::Trigger::Load::TextArea>("load_textarea_" + std::to_string(i));
						for (auto i = 0; i < max_boxes; ++i)
							ctigo_box->e_game_info_->add_id_component<Component::Trigger::Load::Box>("load_box_" + std::to_string(i));

						auto& c_trigger_tree = *entity_->add_id_component<Component::TriggerTree>("trigger_tree");
						auto csu_tree_traverse = entity_->add_id_component<Component::System::Update::TraverseTree>("traverse_tree", c_trigger_tree);

						auto cs_pause_item = e_pause->add_id_component<Component::System::IItem>("item", std::vector<Component::ISystem*>{csu_tree_traverse});
						c_pause_update_engine.add(cs_pause_item, update_group);

						auto ct_load_textbox = e_game_info_->add_id_component<Component::Trigger::Load::TextBox>("load", entity_);
					}
				};
			}
		}
	}
}