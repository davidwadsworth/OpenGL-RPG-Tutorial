#pragma once
#include "component_vector.h"
#include "component_trigger_input_game_obj.h"
#include "component_system.h"
#include <sstream>
#include <vector>
#include "json.hpp"
#include "component_trigger_switch_systems.h"
#include "component_trigger_input_game_obj_empty.h"
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
					std::size_t render_group_, update_group_, max_chars_;
				public:
					TextBox(std::string name, std::string path, glm::vec2 pos, std::size_t render_group, std::size_t update_group)
						: Component::Trigger::Input::IGameObj(name), path_(path), render_group_(render_group), update_group_(update_group), pos_(pos)
					{}

				private:
					void init(Entity* gamestate) override final
					{

						auto e_pause = new Entity();
						entity_->add_id_child(e_pause, "pause");
						auto& c_pause_update_systems = *e_pause->add_id_component<Component::Engine>("update");
						auto ct_pause_switch_systems = e_pause->add_id_component<Component::Trigger::SwitchSystems>("switch_systems", c_pause_update_systems);

						auto ctigo_box = entity_->add_id_component<Component::Trigger::Input::GameObj::Box>("box", "box", render_group_);
						ctigo_box->set_parent(entity_);
						ctigo_box->execute(gamestate);

						auto ctigo_empty = entity_->add_id_component<Component::Trigger::Input::GameObj::Empty>("empty", "textarea", )
						

						auto& c_trigger_tree = *entity_->add_id_component<Component::TriggerTree>("trigger_tree");
						auto csu_tree_traverse = entity_->add_id_component<Component::System::Update::TraverseTree>("traverse_tree", c_trigger_tree);

						auto cs_pause_item = e_pause->add_id_component<Component::System::IItem>("item", std::vector<Component::ISystem*>{csu_tree_traverse});;
					
					}
				};
			}
		}
	}
}