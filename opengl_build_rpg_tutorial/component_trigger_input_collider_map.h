#pragma once
#include "component_trigger_input.h"
#include "component_system_render_camera_draw.h"
#include "component_collider_aabb.h"
#include "component_vector.h"

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
				void create(EntityMap& map) override
				{
					// get collider tile
					auto& c_tset_material = *map["TestTileMap"]->get_component<Component::Material>(0);
					auto& c_tset_col_tile_src = *map["TestTileMap"]->get_component<Component::Src>(67);

					// get map objects
					auto& c_renderer = *map["renderer"]->get_component<Component::Renderer>();

					auto& c_cworld_col_vec = *map["collision world"]->get_component<Component::ColliderVector>();
					auto& c_engine_render_systems = *map["engine"]->get_component<Component::SystemVector>("render");

					// test colliders here
					auto& c_cmap_dest = *entity_->add_component<Component::Dest>();
					auto& c_cmap_transform = *entity_->add_component<Component::Transform>(300.0f, 120.0f, 64.0f);
					
					auto c_cmap_aabb = entity_->add_component<Component::Collider::AABB>(c_cmap_transform, 64.0f);

					auto csr_cmap_cam_draw = entity_->add_component<Component::System::Render::CameraDraw>();

					// add colliders to collision world
					c_cworld_col_vec.push_back(c_cmap_aabb);

					// add render systems
					c_engine_render_systems.push_back(csr_cmap_cam_draw);
				}
			};
		}
	}
}
