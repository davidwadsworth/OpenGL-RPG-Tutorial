#pragma once
#pragma once
#include "component_trigger_input.h"
#include "component_trigger_input_dependent_box.h"
#include "component_trigger_input_dependent_text_area.h"
#include "component_trigger_add_game_obj.h"
#include "component_trigger_switch_systems.h"

/*
Test class to add and use gameobjects

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace Dependent
			{
				class Sandbox : public Component::Trigger::IInput
				{
				public:
					Sandbox(std::string name)
						: Component::Trigger::IInput(name)
					{}
				private:
					void create(Entity* gamestate) override final
					{
						std::string msg = "work first try? I should also check to see if new lines work as well, so here's more text.";

						auto ctid_box = entity_->add_component<Component::Trigger::Input::Dependent::Box>("test box", Rect{10.0f, 10.0f, 300.0f, 200.0f}, 10.0f, 2.0f);
						ctid_box->set_parent(entity_);
						ctid_box->execute(gamestate);

						auto ctid_text_area = entity_->add_component<Component::Trigger::Input::Dependent::TextArea>("test text area", msg, Rect{30.0f, 20.0f, 270.0f, 180.0f}, 1.0f, 1.0f);
						ctid_text_area->set_parent(entity_);
						ctid_text_area->execute(gamestate);
						
						auto& render_systems = *gamestate->get_child("engine")->get_component<Component::SystemVector>("render");

						//auto ct_add_test_box = entity_->push_back_component<Component::Trigger::AddGameObj>(entity_->get_child("test box"));
						//auto ct_add_test_text_area = entity_->push_back_component<Component::Trigger::AddGameObj>(entity_->get_child("test text area"));

						//gamestate->get_child("engine")->get_component<Component::TriggerVector>("trigger")->push_back(ct_add_test_box);
						//gamestate->get_child("engine")->get_component<Component::TriggerVector>("trigger")->push_back(ct_add_test_text_area);
					}

				};
			}
		}
	}
}