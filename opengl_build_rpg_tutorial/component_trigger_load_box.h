#pragma once
#include "component_trigger.h"
#include "component_transform.h"

namespace Component {
	namespace Trigger {
		namespace Load
		{
			class Box : public Component::ITrigger
			{
				Entity* box_;
				Rect rect_;
				float corner_size_, box_sc_;
			public:
				Box(Entity* box)
					: box_(box)
				{}

				void load(Rect rect, float corner_size, float box_sc)
				{
					rect_ = rect;
					corner_size_ = corner_size;
					box_sc_ = box_sc;
				}

				void execute(Entity* gamestate) override
				{
					auto box_x = rect_.x;
					auto box_y = rect_.y;
					auto box_w = rect_.w;
					auto box_h = rect_.h;

					auto scaled_corner_size = corner_size_ * box_sc_;

					auto set_transform = [](Component::Transform& transform, Rect rect)
					{
						transform.x = rect.x;
						transform.y = rect.y;
						transform.w = rect.w;
						transform.h = rect.h;
					};

					// box corners transforms
					auto& c_tl_corner_trans = *box_->get_component<Component::Transform>(0);
					set_transform(c_tl_corner_trans, Rect{ box_x, box_y, scaled_corner_size, scaled_corner_size });
					auto& c_tr_corner_trans = *box_->get_component<Component::Transform>(1);
					set_transform(c_tr_corner_trans, Rect{ box_x + box_w - scaled_corner_size, box_y, scaled_corner_size, scaled_corner_size });
					auto& c_bl_corner_trans = *box_->get_component<Component::Transform>(2);
					set_transform(c_bl_corner_trans, Rect{ box_x, box_y + box_h - scaled_corner_size, scaled_corner_size, scaled_corner_size });
					auto& c_br_corner_trans = *box_->get_component<Component::Transform>(3);
					set_transform(c_br_corner_trans, Rect{ box_x + box_w - scaled_corner_size, box_y + box_h - scaled_corner_size, scaled_corner_size, scaled_corner_size });

					// box side transforms
					auto& c_t_side_trans = *box_->get_component<Component::Transform>(4);
					set_transform(c_t_side_trans, Rect{ box_x + scaled_corner_size, box_y, box_w - scaled_corner_size * 2, scaled_corner_size });
					auto& c_l_side_trans = *box_->get_component<Component::Transform>(5);
					set_transform(c_l_side_trans, Rect{ box_x, box_y + scaled_corner_size, scaled_corner_size, box_h - scaled_corner_size * 2 });
					auto& c_b_side_trans = *box_->get_component<Component::Transform>(6);
					set_transform(c_b_side_trans, Rect{ box_x + scaled_corner_size, box_y + box_h - scaled_corner_size, box_w - scaled_corner_size * 2, scaled_corner_size });
					auto& c_r_side_trans = *box_->get_component<Component::Transform>(7);
					set_transform(c_r_side_trans, Rect{ box_x + box_w - scaled_corner_size, box_y + scaled_corner_size, scaled_corner_size, box_h - scaled_corner_size * 2 });

					// center transform
					auto& c_center_trans = *box_->get_component<Component::Transform>(8);
					set_transform(c_center_trans, Rect{ box_x + scaled_corner_size, box_y + scaled_corner_size, box_w - scaled_corner_size * 2, box_h - scaled_corner_size * 2 });
				}
			};
		}
	}
}