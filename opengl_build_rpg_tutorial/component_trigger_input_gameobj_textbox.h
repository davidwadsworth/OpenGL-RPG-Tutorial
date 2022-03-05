#pragma once
#include "component_trigger_input_gameobj_emptytextarea.h"
#include "component_trigger_input_gameobj_box.h"
#include "component_vector.h"
#include "component_trigger_swapvectors.h"
#include "component_tree.h"
#include "component_system_update_traverse_tree.h"
#include "component_trigger_load_message.h"
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
				class TextBox : public Component::Trigger::IInput
				{
				private:
					void create(Entity* gamestate) override final
					{
						auto& textbox_json = gamestate->get_child("index")->get_component<Component::Json>(name_)->json;

						auto& c_update_vec = *entity_->add_id_component<Component::SystemVector>("update");
						auto ct_switch_vectors = entity_->add_id_component<Component::Trigger::SwapVectors>("swap_vectors", c_update_vec);

						// add message and box triggers to gameinfo
						std::string textarea_name = textbox_json["textarea"];
						std::string box_name = textbox_json["box"];

						auto& ctigo_box = *entity_->add_id_ct_input<Component::Trigger::Input::GameObj::Box>(box_name);
						ctigo_box.execute(gamestate, entity_);

						auto& ctigo_empty = *entity_->add_id_ct_input<Component::Trigger::Input::GameObj::EmptyTextArea>(textarea_name);
						ctigo_empty.execute(gamestate, entity_);
						
						auto e_lc = gamestate->get_child("load");
						e_lc->add_id_component<Component::Trigger::Load::TextBox>(name_);
						e_lc->add_id_component<Component::Trigger::Load::Message>(textarea_name);
						e_lc->add_id_component<Component::Trigger::Load::Box>(box_name);

						// get controller 
						auto& c_cont_keyboard = *gamestate->get_child("controller")->get_component<Component::Controller::Keyboard>();

						// get trigger vector
						auto& c_trigger_vec = *gamestate->get_component<Component::TriggerVector>("trigger");

						auto& c_trigger_tree = *entity_->add_id_component<Component::TriggerTree>("trigger_tree");
						auto csu_tree_traverse = entity_->add_id_component<Component::System::Update::TraverseTree>("traverse_tree", c_trigger_tree, c_cont_keyboard, c_trigger_vec);
						c_update_vec.push_back(csu_tree_traverse);

					}
				public:
					using Component::Trigger::IInput::IInput;
				};
			}
		}
	}
}