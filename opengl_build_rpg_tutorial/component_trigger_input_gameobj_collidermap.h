#pragma once
#include "component_trigger_input.h"
#include "component_system_render_camera_draw.h"
#include "component_collider_gjk_physics_circle_smooth.h"
#include "component_collider_gjk_physics_boundary_smooth.h"
#include "component_collider_gjk_physics_polygon_smooth.h"
#include "component_vector.h"

/*
storage class for tiled collision objects

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			namespace GameObj
			{
				class ColliderMap : public Component::Trigger::IInput
				{
				private:
					void create(Entity* gamestate) override final
					{
						//TODO
					}
				};
			}
		}
	}
}
