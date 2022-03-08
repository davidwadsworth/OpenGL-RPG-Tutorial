#pragma once
#include "component_system.h"
#include "component_parseaction.h"
#include "component_rect.h"
#include "component_controller.h"
#include "component_quadtree.h"
#include "gjk.h"

namespace Component {
	namespace System {
		namespace Update
		{
			class CheckAction : public Component::ISystem
			{
				Component::IController& c_controller_;
				Component::Transform& c_transform_;
				Component::PhysicsActionGJKQTree& c_action_qtree_;
				Component::ParseAction& c_parse_action_;
				glm::vec2 direction_;
				float distance_;
			public:
				CheckAction(Component::IController& c_controller, Component::Transform& c_transform, Component::PhysicsActionGJKQTree& c_action_qtree,
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
						auto distance_pos = glm::vec2(c_transform_.w / 2.0f, c_transform_.h / 2.0f);
						distance_pos.x += c_transform_.x;
						distance_pos.y += c_transform_.y;
						distance_pos += direction_ * distance_;
						Rect rect_a( distance_pos, 0.0f);

						auto retrieved_actions = c_action_qtree_.retrieve(rect_a);

						for (auto rect_b : retrieved_actions)
						{
							if (GJK::collide(distance_pos, *rect_b))
								c_parse_action_.parse(rect_b->action);
						}
					}
				}
			};
		}
	}
}