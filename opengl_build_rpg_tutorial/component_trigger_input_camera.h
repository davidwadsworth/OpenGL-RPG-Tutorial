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
				GLfloat width_, height_;
			public:
				Camera(std::string name, GLfloat width, GLfloat height)
					: Component::Trigger::IInput(name), width_(width), height_(height)
				{}
				Camera(std::string name, GLfloat length)
					: Camera(name, length, length)
				{}
			private:
				void create(Entity* gamestate) override final
				{
					entity_->add_component<Component::Transform>(Rect{ 0.0f, 0.0f, width_, height_ });
				}

			};
		}
	}
}