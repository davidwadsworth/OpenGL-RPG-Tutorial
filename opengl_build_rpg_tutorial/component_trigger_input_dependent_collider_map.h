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
			namespace Dependent
			{

				class ColliderMap : public Component::Trigger::IInput
				{
				public:
					ColliderMap(std::string name)
						: Component::Trigger::IInput(name)
					{}

				private:
					void create(Entity* gamestate) override
					{
						/*
							71 = circle
							48 = boundary
							24 = polygon
							47 = aabb
						*/

						// get collider tile info
						auto& c_tset_material = *gamestate->get_child("TestTilesetGJK")->get_component<Component::Material>(0);

						auto& c_tset_col_circle_src = *gamestate->get_child("TestTilesetGJK")->get_component<Component::Src>(71);
						auto& c_tset_col_boundary_src = *gamestate->get_child("TestTilesetGJK")->get_component<Component::Src>(48);
						auto& c_tset_col_polygon_src = *gamestate->get_child("TestTilesetGJK")->get_component<Component::Src>(24);
						auto& c_tset_col_aabb_src = *gamestate->get_child("TestTilesetGJK")->get_component<Component::Src>(47);

						// get map objects
						auto& c_renderer = *gamestate->get_child("renderer")->get_component<Component::Renderer>();

						auto& c_cworld_col_vec = *gamestate->get_child("collision world")->get_component<Component::GJKVector>();
						auto& c_engine_render_systems = *gamestate->get_child("engine")->get_component<Component::SystemVector>("render");
						auto& c_cam_transform = *gamestate->get_child("camera")->get_component<Component::Transform>();

						// test colliders here

						// gjk circle
						auto& c_circle_transform = *entity_->push_back_component<Component::Transform>(704.0f, 792.0f, 64.0f);
						auto ccgpc_circle_smooth = entity_->push_back_component<Component::Collider::GJK::Physics::Circle::Smooth>
							(c_circle_transform, 32.0f, glm::vec2(32.0f, 32.0f));

						auto csr_circle_cam_draw = entity_->push_back_component<Component::System::Render::CameraDraw>
							(c_renderer, c_tset_col_circle_src, c_circle_transform, c_tset_material, c_cam_transform);

						// gjk boundary
						auto& c_boundary_transform = *entity_->push_back_component<Component::Transform>(704.0f - 128.0f, 792.0f, 64.0f);
						auto ccgpb_boundary_smooth = entity_->push_back_component<Component::Collider::GJK::Physics::Boundary::Smooth>
							(c_boundary_transform, std::array<glm::vec2, MAX_BOUNDARY>{glm::vec2(0.0f, 64.0f), glm::vec2(64.0f, 0.0f)});

						auto csr_boundary_cam_draw = entity_->push_back_component<Component::System::Render::CameraDraw>
							(c_renderer, c_tset_col_boundary_src, c_boundary_transform, c_tset_material, c_cam_transform);

						// gjk polygon
						auto& c_polygon_transform = *entity_->push_back_component<Component::Transform>(704.0f - 256.0f, 792.0f, 64.0f);
						auto ccgpp_polygon_smooth = entity_->push_back_component<Component::Collider::GJK::Physics::Polygon::Smooth>
							(c_polygon_transform, std::vector<glm::vec2>{glm::vec2(0.0f), glm::vec2(64.0f, 64.0f), glm::vec2(0.0f, 64.0f)});

						auto csr_polygon_cam_draw = entity_->push_back_component<Component::System::Render::CameraDraw>
							(c_renderer, c_tset_col_polygon_src, c_polygon_transform, c_tset_material, c_cam_transform);


						// add colliders to collision world
						c_cworld_col_vec.push_back(ccgpc_circle_smooth);
						c_cworld_col_vec.push_back(ccgpb_boundary_smooth);
						c_cworld_col_vec.push_back(ccgpp_polygon_smooth);

						// add render systems
						c_engine_render_systems.push_back(csr_circle_cam_draw);
						c_engine_render_systems.push_back(csr_boundary_cam_draw);
						c_engine_render_systems.push_back(csr_polygon_cam_draw);
					}
				};
			}
		}
	}
}
