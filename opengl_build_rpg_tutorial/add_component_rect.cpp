#include "add_component_rect.h"
#include "component_system_render_cameradraw.h"
#include "component_quadtree.h"
#include "component_engine.h"
#include "component_rect_action.h"
#include "component_json.h"
#include "component_rect_gjk_physics_smooth.h"

Component::Rectang* add_component_rect(Entity* entity, Entity* gamestate, nlohmann::json info_json)
{
	float x = info_json["rect"]["x"];
	float y = info_json["rect"]["y"];
	float w = info_json["rect"]["w"];
	float h = info_json["rect"]["h"];

	bool has_action = info_json.contains("action");
	bool has_collider = info_json.contains("collider");
	bool has_tree = info_json.contains("tree");

	Rect in_rect(x, y, w, h);
	if (has_collider)
	{
		std::string collider_filename = info_json["collider"]["filename"];
		std::string collider_name = info_json["collider"]["id"];
		int collider_pos = info_json["collider"]["pos"];
		auto collider_json = gamestate->get_child("index")->
			get_component<Component::Json>(collider_filename)->json;
		bool debug = collider_json["debug"] == "true";

		Component::Rectangle::IGJK* rect = nullptr;

		if (collider_name == "circle")
		{
			auto circle_json = collider_json[collider_name][collider_pos];
			float radius = circle_json["radius"];
			float center_x = circle_json["center"]["x"];
			float center_y = circle_json["center"]["y"];
			if (has_action)
			{
				auto ccgjkaps_circle = entity->push_back_component
					<Component::Rectangle::GJK::Physics::Circle::SmoothPhysicsAction>
					(in_rect, radius, glm::vec2(center_x, center_y));
				ccgjkaps_circle->action = info_json;
				rect = ccgjkaps_circle;
			}
			else
			{
				auto crgjkbps_polygon = entity->push_back_component
				<Component::Rectangle::GJK::Physics::Circle::SmoothPhysics>
				(in_rect, radius, glm::vec2(center_x, center_y));
				rect = crgjkbps_polygon;
			}
		}
		else if (collider_name == "boundary")
		{
			auto boundary_json = collider_json[collider_name][collider_pos];
			auto points = boundary_json["points"];

			std::array<glm::vec2, MAX_BOUNDARY> arr_points{ glm::vec2(points["p1"]["x"], points["p1"]["y"]),
				glm::vec2(points["p2"]["x"], points["p2"]["y"]) };

			if (has_action)
			{
				auto ccgjkaps_boundary = entity->push_back_component
					<Component::Rectangle::GJK::Physics::Boundary::SmoothPhysicsAction>(in_rect, arr_points);
				ccgjkaps_boundary->action = info_json;
				rect = ccgjkaps_boundary;
			}
			else
			{
				auto crgjkbps_boundary = entity->push_back_component
					<Component::Rectangle::GJK::Physics::Boundary::SmoothPhysics>(in_rect, arr_points);
				rect = crgjkbps_boundary;
			}
		}
		else if (collider_name == "polygon")
		{
			auto& polygon_json = collider_json[collider_name][collider_pos];
			auto& points = polygon_json["points"];
			std::vector<glm::vec2> vec2_points;
			for (auto& p : points)
			{
				float x = p["x"];
				float y = p["y"];
				vec2_points.push_back(glm::vec2(x, y));
			}

			if (has_action)
			{
				auto ccgjkaps_polygon = entity->push_back_component
					<Component::Rectangle::GJK::Physics::Polygon::SmoothPhysicsAction>(in_rect, vec2_points);
				ccgjkaps_polygon->action = info_json;
				rect = ccgjkaps_polygon;
			}
			else
			{
				auto crgjkpsp_polygon = entity->push_back_component<Component::Rectangle::GJK::Physics::Polygon::SmoothPhysics>
					(in_rect, vec2_points);
				rect = crgjkpsp_polygon;
			}
		}
		else
			Logger::error("invalid collider name: " + info_json.dump(), Logger::HIGH);

		if (debug)
		{
			// get renderer
			auto& c_renderer = *gamestate->get_component<Component::Renderer>("renderer");

			// get camera
			auto& c_cam_position = *gamestate->get_child("camera")->get_component<Component::Position>();

			// get render engine
			auto& c_render_engine = *gamestate->get_component<Component::Engine>("render");
		
			int tileid = collider_json[collider_name][collider_pos]["tileid"];
			std::string tileset_name = collider_json["tileset"];
			float render_group = collider_json["render_group"];

			// set up collider draw
			auto& c_tset_material = *gamestate->get_child(tileset_name)->get_component<Component::Material>("material");
			auto& c_tset_col_src = *gamestate->get_child(tileset_name)->get_child("tiles")->get_component<Component::Src>(tileid);

			auto csr_col_cam_draw = entity->push_back_component<Component::System::Render::CameraDraw>
				(c_renderer, c_tset_col_src, *rect, c_tset_material, c_cam_position);

			c_render_engine.add(csr_col_cam_draw, render_group);
		}

		if (has_tree)
		{
			std::string quadtree_name = info_json["tree"];
			auto e_quadtree = gamestate->get_child(quadtree_name);
			if (has_action)
				Component::PhysicsActionGJKQTree::add(dynamic_cast<Component::Rectangle::GJK::PhysicsAction*>(rect), e_quadtree);
			else
				Component::PhysicsGJKQTree::add(dynamic_cast<Component::Rectangle::GJK::PhysicsNorm*>(rect), e_quadtree);
		}

		return rect;
	}
	else
	{
		if (has_action)
		{
			std::string quadtree_name = info_json["tree"];
			auto e_quadtree = gamestate->get_child(quadtree_name);
			auto cr_action = entity->push_back_component<Component::Rectangle::Action>(in_rect);
			cr_action->action = info_json;
			Component::ActionQTree::add(cr_action, e_quadtree);
			return cr_action;
		}
		else
			return entity->push_back_component<Component::Rectang>(in_rect);
	}
}