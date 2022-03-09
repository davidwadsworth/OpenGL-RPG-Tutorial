#pragma once
#include "component_system.h"
#include "component_rect.h"
#include "component_controller.h"
#include "component_quadtree.h"
#include "gjk.h"
#include "component_trigger_load.h"
#include "component_vector.h"

namespace Component {
	namespace System {
		namespace Update
		{
			class CheckAction : public Component::ISystem
			{
				Component::IController& c_controller_;
				Component::Transform& c_transform_;
				Component::PhysicsActionGJKQTree& c_action_qtree_;
				Entity* e_load_;
				Component::TriggerVector& c_triggervec_;
				glm::vec2 direction_;
				float distance_;
			public:
				CheckAction(Component::IController& c_controller, Component::Transform& c_transform, Component::PhysicsActionGJKQTree& c_action_qtree,
					Entity* e_load, Component::TriggerVector& c_trigger, float distance)
					: c_controller_(c_controller), c_transform_(c_transform), c_action_qtree_(c_action_qtree), 
					e_load_(e_load), c_triggervec_(c_trigger), direction_(0.0f, 1.0f), distance_(distance)
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
							{
								for (auto action : rect_b->action)
								{
									std::string load = action["load"];

									std::stringstream ss;
									ss << "{\"x\":" << rect_b->x << ",\"y\":" << rect_b->y << ",\"w\":"
										<< rect_b->w << ",\"h\":" << rect_b->h << "}";

									action["data"]["rect"] = nlohmann::json::parse(ss);

									auto ct_load = e_load_->get_component<Component::Trigger::ILoad>(load);
									ct_load->load(action["data"]);

									c_triggervec_.push_back(ct_load);
								}
							}
						}
					}
				}
			};
		}
	}
}