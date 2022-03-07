#pragma once
#include "component_system.h"
#include "component_parseaction.h"
#include "component_rect.h"
#include "component_controller.h"
#include "component_quadtree.h"
#include "aabb.h"

namespace Component {
	namespace System {
		namespace Update
		{
			class CheckAction : public Component::ISystem
			{
				Component::IController& c_controller_;
				Component::Transform& c_transform_;
				Component::PhysicsActionQTree& c_action_qtree_;
				Component::ParseAction& c_parse_action_;
				glm::vec2 direction_;
				float distance_;
			public:
				CheckAction(Component::IController& c_controller, Component::Transform& c_transform, Component::PhysicsActionQTree& c_action_qtree,
					Component::ParseAction& c_parse_action, float distance)
					: c_controller_(c_controller), c_transform_(c_transform), c_action_qtree_(c_action_qtree), 
					c_parse_action_(c_parse_action), direction_(0.0f, 1.0f), distance_(distance)
				{}

				void execute()
				{
					// get the current state of movement for our character to find what direction he's facing
					auto controller_state = glm::vec2{ (float)c_controller_.key_down_right() - (float)c_controller_.key_down_left(),
					(float)c_controller_.key_down_down() - (float)c_controller_.key_down_up() };

					if (controller_state.x || controller_state.y)
						direction_ = controller_state;
					
					if (c_controller_.key_press_action_1())
					{
						auto center_point = glm::vec2(c_transform_.w / 2.0f, c_transform_.h / 2.0f);
						center_point.x += c_transform_.x;
						center_point.y += c_transform_.y;
						Rect rect_a(direction_ * distance_ + center_point, 0.0f);

						auto retrieved_actions = c_action_qtree_.retrieve(rect_a);

						for (auto rect_b : retrieved_actions)
						{
							if (AABB::collide(rect_a, *rect_b))
								c_parse_action_.parse(rect_b->action);
						}
					}
				}
			};
		}
	}
}