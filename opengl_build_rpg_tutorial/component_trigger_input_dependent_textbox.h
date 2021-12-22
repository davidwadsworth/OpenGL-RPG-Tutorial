#pragma once
#include "component_vector.h"
#include "component_trigger_input.h"
#include "component_system.h"
#include <sstream>
#include <vector>
#include "json.hpp"
#include "component_template.h"
#include "component_material.h"
#include "component_system_render_camera_draw.h"
#include "component_controller_keyboard.h"
#include "component_src_bitmap_glyph.h"
#include "component_trigger_input_dependent_text_area.h"
#include "component_trigger_input_dependent_box.h"

/*
Holding class to draw textboxes onto the screen

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace Dependent
			{
				class TextBox : public Component::Trigger::IInput
				{
				public:
					TextBox(std::string name)
						: Component::Trigger::IInput(name)
					{}

				private:
					void create(Entity* gamestate) override final
					{
						std::string msg = "work first try? I should also check to see if new lines work as well, so here's more text. To finish off this experiment lets also add a dummy amount of text to see if it caps off textboxes correctly";

						auto ctid_box = entity_->add_component<Component::Trigger::Input::Dependent::Box>("test box", Rect{ 10.0f, 10.0f, 300.0f, 200.0f }, 10.0f, 2.0f);
						ctid_box->set_parent(entity_);
						ctid_box->execute(gamestate);

						auto ctid_text_area = entity_->add_component<Component::Trigger::Input::Dependent::TextArea>("test text area", msg, Rect{ 30.0f, 20.0f, 260.0f, 160.0f }, 1.0f, 1.0f, "middle", "top ");
						ctid_text_area->set_parent(entity_);
						ctid_text_area->execute(gamestate);						
					}
				};
			}
		}
	}
}