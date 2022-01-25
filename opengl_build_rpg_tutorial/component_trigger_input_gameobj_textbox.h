#pragma once
#include "component_vector.h"
#include "component_system.h"
#include <sstream>
#include <vector>
#include "json.hpp"
#include "component_trigger_swapengine.h"
#include "component_trigger_input_gameobj_empty.h"
#include "component_trigger_input_gameobj_box.h"
#include "component_tree.h"
#include "component_system_update_traverse_tree.h"
#include "component_trigger_loadtextbox.h"

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
					std::string font_;
					std::size_t render_group_, update_group_, max_chars_;
				public:
					TextBox(std::string name, std::string font, std::size_t max_chars, std::size_t render_group, std::size_t update_group)
						: Component::Trigger::Input::IGameObj(name), font_(font), render_group_(render_group), update_group_(update_group), max_chars_(max_chars)
					{}

				private:
					void init(Entity* gamestate) override final
					{
						auto e_pause = new Entity();
						entity_->add_id_child(e_pause, "pause");
						auto& c_pause_update_engine = *e_pause->add_id_component<Component::Engine>("update");
						auto ct_pause_switch_systems = e_pause->add_id_component<Component::Trigger::SwapEngine>("switch_systems", c_pause_update_engine);

						auto ctigo_box = entity_->add_id_ct_input<Component::Trigger::Input::GameObj::Box>("box", render_group_);
						ctigo_box->set_parent(entity_);
						ctigo_box->execute(gamestate);

						auto ctigo_empty = entity_->add_id_ct_input<Component::Trigger::Input::GameObj::Empty>("empty", font_, max_chars_, render_group_);
						ctigo_empty->set_parent(entity_);
						ctigo_empty->execute(gamestate);
						
						auto& c_trigger_tree = *entity_->add_id_component<Component::TriggerTree>("trigger_tree");
						auto& csu_tree_traverse = *entity_->add_id_component<Component::System::Update::TraverseTree>("traverse_tree", c_trigger_tree);

						auto cs_pause_item = e_pause->add_id_component<Component::System::IItem>("item", std::vector<Component::ISystem*>{csu_tree_traverse});;
						c_pause_update_engine.add(cs_pause_item, update_group_);

						auto ct_load_textbox = e_game_info_->add_id_component<Component::Trigger::LoadTextBox>("add", csu_tree_traverse, entity_->get_child("box"), entity_->get_child("text"));
					}
				};
			}
		}
	}
}