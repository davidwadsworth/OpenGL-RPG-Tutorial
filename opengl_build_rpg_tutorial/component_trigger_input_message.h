#pragma once
#include "component_trigger_input.h"
#include "delimiter_split.h"
#include "component_json.h"
#include "component_collider_gjk_boundary.h"
#include "component_collider_gjk_polygon.h"
#include "component_collider_gjk_circle.h"
#include "component_collider_aabb.h"
#include "component_material.h"
#include "component_system_render_camera_draw.h"
#include "component_quadtree.h"
#include "component_engine.h"

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class Message : public IInput
			{
				void create(Entity* gamestate)
				{
					auto message_name = delimiter_split(name_, '_')[0];
					auto message_json = gamestate->get_child("index")->get_component<Component::Json>()->json;
					auto e_action_tree = gamestate->get_child("action_tree");
					auto e_collision_tree = gamestate->get_child("collision_tree");

					// get renderer
					auto& c_renderer = *gamestate->get_component<Component::Renderer >("renderer");

					// get camera
					auto& c_cam_position = *gamestate->get_child("camera")->get_component<Component::Position>();

					// get collision world
					auto& c_colw_col_vec = *gamestate->get_child("collision_world")->get_component<Component::QuadTree>();
					
					// get render engine
					auto& c_render_engine = *gamestate->get_component<Component::Engine>("render_engine");

					for (auto msg : message_json)
					{
						float x = msg["position_x"];
						float y = msg["position_y"];
						std::vector<std::string> collider_info = msg["collider"];
						bool collidable = msg["collidable"] == "true";
						std::vector<std::string> textbox_info = msg["textbox"];
						std::vector<std::string> texture_info = msg["signpost"];
						float render_group = msg["render_group"];

						if (collider_info.size() != 3)
							Logger::error("collider info wrong: " + msg.dump(), Logger::HIGH);

						Component::Rectangle* rect;
						float w, h;
						int col_pos = stoi(collider_info[2]);
						auto collider_json = gamestate->get_child("index")->get_component<Component::Json>(collider_info[0])->json;
						bool debug = collider_json["debug"] == "true";

						if (collider_info[1] == "aabb")
						{
							auto aabb_json = collider_json[collider_info[1]][col_pos];
							w = aabb_json["width"];
							h = aabb_json["height"];
							rect = entity_->push_back_component<Component::Collider::AABB>(Rect(x, y, w, h));
						}
						else
						{
							w = msg["width"];
							h = msg["height"];
							if (collider_info[1] == "circle")
							{
								auto circle_json = collider_json[collider_info[1]][col_pos];
								float radius = circle_json["radius"];
								float center_x = circle_json["center"][0];
								float center_y = circle_json["center"][1];
								rect = entity_->push_back_component<Component::Collider::GJK::Circle>(Rect(x, y, w, h), radius, glm::vec2(center_x, center_y));
							}
							else if (collider_info[1] == "boundary")
							{
								auto boundary_json = collider_json[collider_info[1]][col_pos];
								std::vector<float> points = boundary_json["points"];
								if (points.size() != 4)
									Logger::error("Something wrong with boundary flatfile: " + msg.dump(), Logger::HIGH);
								std::array<glm::vec2, 2> arr_points{ glm::vec2(points[0], points[1]), glm::vec2(points[2], points[3]) };
								rect = entity_->push_back_component<Component::Collider::GJK::Boundary>(Rect(x, y, w, h), arr_points);
							}
							else if (collider_info[1] == "polygon")
							{
								auto polygon_json = collider_json[collider_info[1]][col_pos];
								std::vector<float> points = polygon_json["points"];
								if (points.size() % 2 != 0)
									Logger::error("Odd number of points with polygon flatfile: " + msg.dump(), Logger::HIGH);
								std::vector<glm::vec2> vec2_points;
								for (auto i = 0; i + 1 < points.size(); i += 2)
									vec2_points.push_back(glm::vec2(vec2_points[i], vec2_points[i + 1]));

								rect = entity_->push_back_component<Component::Collider::GJK::Polygon>(Rect(x, y, w, h), vec2_points);
							}
							else
								Logger::error("invalid collider name: " + msg.dump(), Logger::HIGH);

							if (debug)
							{
								int tileid = collider_json[collider_info[0]]["tileid"];
								std::string tileset_name = collider_json["tileset"];

								// set up collider draw
								auto& c_tset_material = *gamestate->get_child(tileset_name)->get_component<Component::Material>(0);
								auto& c_tset_col_circle_src = *gamestate->get_child(tileset_name)->get_component<Component::Src>(tileid);

								auto csr_col_cam_draw = entity_->push_back_component<Component::System::Render::CameraDraw>(c_renderer, c_tset_col_circle_src, *rect, c_tset_material, c_cam_position);

								c_render_engine.add(csr_col_cam_draw, render_group);
							}
						}

						if (collidable)
							Component::QuadTree::add(rect, e_collision_tree);

					}
				}
				
			};
		}
	}
}