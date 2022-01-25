#pragma once
#include "component_trigger_input_game_obj.h"
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

				class ColliderMap : public Component::Trigger::Input::IGameObj
				{
					std::size_t render_group_;
				public:
					ColliderMap(std::string name, std::size_t render_group)
						: Component::Trigger::Input::IGameObj(name), render_group_(render_group)
					{}

				private:
					void init(Entity* gamestate) override final
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
						auto& c_renderer = *gamestate->get_component<Component::Renderer>("renderer");

						auto& c_cworld_col_vec = *gamestate->get_child("collision world")->get_component<Component::GJKVector>();
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

						std::vector<Component::ISystem*> temp_systems;

						temp_systems.push_back(csr_circle_cam_draw);
						temp_systems.push_back(csr_boundary_cam_draw);
						temp_systems.push_back(csr_polygon_cam_draw);

						// add render systems to game obj info 
						auto csi_render_item = e_game_info_->add_id_component<Component::System::IItem>("render", temp_systems);
						gamestate->get_component<Component::Engine>("render")->add(csi_render_item, render_group_);
					}
				};
			}
		}
	}
}
