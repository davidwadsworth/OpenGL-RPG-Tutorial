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
			template <typename T>
			class QuadTree : public Component::Trigger::IInput
			{
			public:
				using Component::Trigger::IInput::IInput;
			private:
				void create(Entity* gamestate) override
				{
					auto& qt_json = Game::global->get_child("index")->get_component<Component::Json>("quadtree")->json;

					float x = qt_json["rect"]["x"];
					float y = qt_json["rect"]["y"];
					float w = qt_json["rect"]["w"];
					float h = qt_json["rect"]["h"];
					int max_objects = qt_json["max_objects"];

					this->entity_->push_back_component<Component::QuadTree<T>>(max_objects, Rect(x, y, w, h));
				}
			};
		}
	}
}
