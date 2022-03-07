#include "add_component_rect.h"
#include "component_system_render_cameradraw.h"
#include "component_quadtree.h"
#include "component_engine.h"
#include "component_rect_action.h"
#include "component_json.h"

Component::Rectang* add_component_rect(Entity* entity, Entity* gamestate, nlohmann::json info_json)
{

	float x = info_json["rect"]["x"];
	float y = info_json["rect"]["y"];
	float w = info_json["rect"]["w"];
	float h = info_json["rect"]["h"];

	Component::Rectang* rect = nullptr;
	bool has_action = info_json.contains("action");
	bool has_collider = info_json.contains("collider");

	Rect in_rect(x, y, w, h);
	if (has_collider)
	{
		std::string collider_filename = info_json["collider"]["filename"];
		std::string collider_name = info_json["collider"]["id"];
		int collider_pos = info_json["collider"]["pos"];
		auto collider_json = gamestate->get_child("index")->
			get_component<Component::Json>(collider_filename)->json;
		bool debug = collider_json["debug"] == "true";

		if (collider_name == "circle")
		{
			auto circle_json = collider_json[collider_name][collider_pos];
			float radius = circle_json["radius"];
			float center_x = circle_json["center"]["x"];
			float center_y = circle_json["center"]["y"];
			if (has_action)
			{
				auto ccgjkaps_circle = entity->push_back_component
					<Component::Collider::GJK::Action::Physics::Smooth::Circle>
					(in_rect, radius, glm::vec2(center_x, center_y));
				ccgjkaps_circle->action = info_json["action"];
				rect = ccgjkaps_circle;
			}
			else
				rect = entity->push_back_component
				<Component::Collider::GJK::Physics::Circle::Smooth>
				(in_rect, radius, glm::vec2(center_x, center_y));
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
					<Component::Collider::GJK::Action::Physics::Smooth::Boundary>(in_rect, arr_points);
				ccgjkaps_boundary->action = info_json["action"];
				rect = ccgjkaps_boundary;
			}
			else
				rect = entity->push_back_component
				<Component::Collider::GJK::Physics::Boundary::Smooth>(in_rect, arr_points);
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
					<Component::Collider::GJK::Action::Physics::Smooth::Polygon>(in_rect, vec2_points);
				ccgjkaps_polygon->action = info_json["action"];
				rect = ccgjkaps_polygon;
			}
			else
				rect = entity->push_back_component<Component::Collider::GJK::Physics::Polygon::Smooth>
				(in_rect, vec2_points);
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
	}
	else
	{
		if (has_action)
		{
			auto cr_action = entity->push_back_component<Component::ARect>(in_rect);
			cr_action->action = info_json["action"];
			rect = cr_action;
		}
		else
			rect = entity->push_back_component<Component::Rectangle>(in_rect);
	}

	if (has_collider)
	{
		std::string tree_name = info_json["collider"]["tree"];
		auto e_quadtree = gamestate->get_child(tree_name);
		Component::QuadTree::add(rect, e_quadtree);
	}

	if (has_action)
	{
		std::string tree_name = info_json["action"]["tree"];
		auto e_quadtree = gamestate->get_child(tree_name);
		Component::QuadTree::add(rect, e_quadtree);
	}

	return rect;
}