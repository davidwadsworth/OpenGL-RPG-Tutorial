#pragma once
#include "component_system.h"
#include "component_vector.h"
#include "component_rect_gjk.h"
#include "component_quadtree.h"
#include <sstream>
#include "gjk.h"
#include "component_system_update_checkaction.h"

/*
System used for checking for and resolving collisions, in general space around the specified object

@author David Wadsworth
*/

namespace Component {
	namespace System {
		namespace Update
		{
			class CheckActionCollision : public Component::ISystem
			{
				Component::Rectangle::IGJK& rect_a_;
				Component::ActionGJKQTree& c_quad_tree_;
				Component::TriggerVector& c_triggervec_;
				Entity* e_load_;
			public:
				CheckActionCollision(Component::Rectangle::IGJK& rect_a, Component::ActionGJKQTree& c_quad_tree,
					Component::TriggerVector& c_triggervec, Entity* e_load)
					: rect_a_(rect_a), c_quad_tree_(c_quad_tree), c_triggervec_(c_triggervec), e_load_(e_load)
				{}

				void execute() override
				{
					auto retrieved_rect = c_quad_tree_.retrieve(rect_a_);

					for (auto rect_b : retrieved_rect)
						if (GJK::collide(rect_a_, *rect_b))
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
			};
		}
	}
}