#pragma once
#include "component_trigger_input.h"
#include "component_system_render_camera_draw.h"
#include "component_collider_physics_aabb_smooth.h"
#include "component_collider_physics_aabb_sticky.h"
#include "component_vector.h"

/*
storage class for tiled collision objects

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class ColliderMap : public Component::Trigger::In
			{
			public:
				ColliderMap(std::string name)
					: Component::Trigger::In(name)
				{}

			private:
				void create(Entity* gamestate) override
				{
					// get collider tile info
					auto& c_tset_material = *gamestate->get_child("TestTileset")->get_component<Component::Material>(0);
					auto& c_tset_col_tile_src = *gamestate->get_child("TestTileset")->get_component<Component::Src>(46);

					// get map objects
					auto& c_renderer = *gamestate->get_child("renderer")->get_component<Component::Renderer>();

					auto& c_cworld_col_vec = *gamestate->get_child("collision world")->get_component<Component::ColliderVector>();
					auto& c_engine_render_systems = *gamestate->get_child("engine")->get_component<Component::SystemVector>("render");
					auto& c_cam_transform = *gamestate->get_child("camera")->get_component<Component::Transform>();

					// test colliders here

					// smooth aabb
					auto& c_smaabb_dest = *entity_->push_back_component<Component::Dest>();
					auto& c_smaabb_transform = *entity_->push_back_component<Component::Transform>(704.0f, 600.0f, 64.0f);
					auto c_smaabb_aabb = entity_->push_back_component<Component::Collider::Physics::AABBSmooth>(c_smaabb_transform, 64.0f);

					auto csr_smaabb_cam_draw = entity_->push_back_component<Component::System::Render::CameraDraw>
						(c_renderer, c_tset_col_tile_src, c_smaabb_dest, c_tset_material, c_smaabb_transform, c_cam_transform);

					// sticky aabb 
					auto& c_staabb_dest = *entity_->push_back_component<Component::Dest>();
					auto& c_staabb_transform = *entity_->push_back_component<Component::Transform>(896.0f, 600.0f, 64.0f);
					auto c_staabb_aabb = entity_->push_back_component<Component::Collider::Physics::AABBSticky>(c_staabb_transform, 64.0f);

					auto csr_staabb_cam_draw = entity_->push_back_component<Component::System::Render::CameraDraw>
						(c_renderer, c_tset_col_tile_src, c_staabb_dest, c_tset_material, c_staabb_transform, c_cam_transform);

					// add colliders to collision world
					c_cworld_col_vec.push_back(c_smaabb_aabb);
					c_cworld_col_vec.push_back(c_staabb_aabb);

					// add render systems
					c_engine_render_systems.push_back(csr_smaabb_cam_draw);
					c_engine_render_systems.push_back(csr_staabb_cam_draw);
				}
			};
		}
	}
}
