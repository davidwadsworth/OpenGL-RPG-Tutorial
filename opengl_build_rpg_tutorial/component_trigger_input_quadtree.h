#pragma once
#include "component_trigger_input.h"
#include "component_quadtree.h"
#include "component_json.h"

/*
List of all active colliding objects in game world.

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class QuadTree : public Component::Trigger::IInput
			{
			public:
				using Component::Trigger::IInput::IInput;
			private:
				void create(Entity* gamestate) override
				{
					auto qt_json = gamestate->get_child("index")->get_component<Component::Json>("quadtree")->json;

					float x = qt_json["rect"][0];
					float y = qt_json["rect"][1];
					float w = qt_json["rect"][2];
					float h = qt_json["rect"][3];
					int max_objects = qt_json["max_objects"];

					this->entity_->add_component<Component::QuadTree>(max_objects, Rect(x, y, w, h));
				}
			};
		}
	}
}
