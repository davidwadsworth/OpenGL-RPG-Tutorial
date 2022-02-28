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
						/*
							71 = circle
							48 = boundary
							24 = polygon
							47 = aabb
						*/

						// get collider tile info
						auto& c_tset_material = *gamestate->get_child("tileset")->get_component<Component::Material>(0);

						std::vector<Component::Src*> src_vec;

						src_vec.push_back(gamestate->get_child("tileset")->get_component<Component::Src>(71));
						src_vec.push_back(gamestate->get_child("tileset")->get_component<Component::Src>(48));
						src_vec.push_back(gamestate->get_child("tileset")->get_component<Component::Src>(24));

						// get map objects
						auto& c_renderer = *gamestate->get_component<Component::Renderer>("renderer");

						auto e_collision_world = gamestate->get_child("collision_world");
						auto& c_cam_position = *gamestate->get_child("camera")->get_component<Component::Position>();

						// test colliders here
						std::vector<Component::Transform*> transform_vec;

						// gjk circle
						auto ccgpc_circle_smooth = entity_->push_back_component<Component::Collider::GJK::Physics::Circle::Smooth>
							(Rect(704.0f, 792.0f, 64.0f), 32.0f, glm::vec2(32.0f, 32.0f));

						transform_vec.push_back(ccgpc_circle_smooth);
						// gjk boundary
						auto ccgpb_boundary_smooth = entity_->push_back_component<Component::Collider::GJK::Physics::Boundary::Smooth>
							(Rect(704.0f - 128.0f, 792.0f, 64.0f), std::array<glm::vec2, MAX_BOUNDARY>{glm::vec2(0.0f, 64.0f), glm::vec2(64.0f, 0.0f)});

						transform_vec.push_back(ccgpb_boundary_smooth);
						// gjk polygon
						auto ccgpp_polygon_smooth = entity_->push_back_component<Component::Collider::GJK::Physics::Polygon::Smooth>
							(Rect(704.0f - 256.0f, 792.0f, 64.0f), std::vector<glm::vec2>{glm::vec2(0.0f), glm::vec2(64.0f, 64.0f), glm::vec2(0.0f, 64.0f)});

						transform_vec.push_back(ccgpp_polygon_smooth);

						// add colliders to collision world
						Component::QuadTree::add(ccgpc_circle_smooth, e_collision_world);
						Component::QuadTree::add(ccgpb_boundary_smooth, e_collision_world);
						Component::QuadTree::add(ccgpp_polygon_smooth, e_collision_world);

						auto csr_offset = entity_->add_id_component<Component::System::Render::Offset>("render", c_cam_position, src_vec, transform_vec, c_tset_material, c_renderer);
						gamestate->get_component<Component::Engine>("render")->add(csr_offset, 3.0f);
					}
				};
			}
		}
	}
}
