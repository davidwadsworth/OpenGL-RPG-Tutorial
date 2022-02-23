#pragma once
#include "component_trigger_input.h"
#include "component_transform.h"
#include "rect.h"
#include <glm/vec2.hpp>

/*
Creates our local camera using width and height

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input 
		{
			class Camera : public Component::Trigger::IInput
			{
			private:
				void create(Entity* gamestate) override final
				{
					auto& camera_json = gamestate->get_child("index")->get_child("camera")->get_component<Component::Json>()->json;

					float width = camera_json["width"];
					float height = camera_json["height"];

					entity_->add_component<Component::Transform>(Rect{ 0.0f, 0.0f, width, height });
				}

			};
		}
	}
}