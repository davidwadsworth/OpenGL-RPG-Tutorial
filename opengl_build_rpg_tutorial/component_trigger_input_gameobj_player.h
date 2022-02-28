#pragma once
#include "component_renderer.h"
#include "component_material.h"
#include "component_system_render_camera_draw.h"
#include "component_system_update_animate_move.h"
#include "component_system_update_animation.h"
#include "component_system_update_move.h"
#include "component_system_update_camera.h"
#include "component_controller_keyboard.h"
#include "component_vector.h"
#include "component_collider_gjk_circle.h"
#include "component_system_update_check_collision.h"
#include "component_engine.h"
#include "component_json.h"

/*
Creates our player object to move around in the overworld.

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
            namespace GameObj
            {
                class Player : public Component::Trigger::IInput
                {
                private:
                    void create(Entity* gamestate) override final
                    {
                        auto player_name = delimiter_split(name_.c_str(), '_')[0]

                        auto& player_json = gamestate->get_child("index")->get_component<Component::Json>(name_)->json;

                        float pos_x = player_json["position"][0];
                        float pos_y = player_json["position"][1];

                        float scale = player_json["scale"];

                        std::string spritesheet_name = player_json["spritesheet"][0];
                        std::string spritesheet_category_name = player_json["spritesheet"][1];

                        std::string collider_name = player_json["collider"][0];
                        std::string collider_category = player_json["collider"][1];
                        int collider_category_pos = player_json["collider"][2];


                        std::vector<nlohmann::json> animation_jsons = player_json["animation"];

                        float animation_speed = player_json["animation_speed"];
                        float player_speed = player_json["player_speed"];
                        float render_group = player_json["render_group"];
                        float update_group = player_json["update_group"];

                        auto& collider_json = gamestate->get_child("index")->get_child(collider_name)->get_component<Component::Json>()->json;

                        bool debug = collider_json["debug"] == "true";

                        auto &shape_json = collider_json[collider_category][collider_category_pos];

                        float circle_radius = shape_json["circle"]["radius"];
                        float circle_center_x = shape_json["circle"]["position"][0];
                        float circle_center_y = shape_json["circle"]["position"][1];

                        auto spritesheet_category = gamestate->get_child(spritesheet_name)->get_child(spritesheet_category_name)->get_component_list();

                        auto& c_idle_down_src = *static_cast<Component::Src*>(spritesheet_category[0]);

                        // get renderer
                        auto& c_renderer = *gamestate->get_component<Component::Renderer >("renderer");

                        // get controller 
                        auto& c_cont_keyboard = *gamestate->get_child("controller")->get_component<Component::Controller::Keyboard>();

                        // get camera
                        auto& c_cam_position = *gamestate->get_child("camera")->get_component<Component::Position>();

                        // get collision world
                        auto& c_colw_col_vec = *gamestate->get_child("collision_world")->get_component<Component::QuadTree>();

                        // get render engine
                        auto& c_render_engine = *gamestate->get_component<Component::Engine>("render_engine");

                        // get update engine
                        auto& c_update_engine = *gamestate->get_component<Component::Engine>("update_engine");

                        auto& c_pla_src = *entity_->add_component<Component::Src>(c_idle_down_src);
                        auto& c_spritesheet_material = *gamestate->get_child(spritesheet_name)->get_component<Component::Material>();

                        auto& c_pla_col_gjk_circle = *entity_->add_component<Component::Collider::GJK::Circle>(Rect(pos_x, pos_y, c_idle_down_src.w), circle_radius, glm::vec2(circle_center_x, circle_center_y));

                        auto csr_pla_dynamic_draw = entity_->add_component<Component::System::Render::CameraDraw>(c_renderer, c_pla_src, c_pla_col_gjk_circle, c_spritesheet_material, c_cam_position);
                        auto csu_pla_camera = entity_->add_component<Component::System::Update::Camera>(c_pla_col_gjk_circle, c_cam_position);
                        auto csu_pla_move = entity_->add_component<Component::System::Update::Move>(c_pla_col_gjk_circle, c_cont_keyboard, player_speed);

                        auto csu_pla_animation = entity_->add_component<Component::System::Update::Animation>(animation_speed, c_pla_src);
                        auto csu_pla_animate_move = entity_->add_component<Component::System::Update::AnimateMove>(c_cont_keyboard, *csu_pla_animation);
                        auto csu_check_collision_gjk = entity_->add_component<Component::System::Update::CheckCollisionGJK>(c_pla_col_gjk_circle, c_colw_col_vec);


                        auto anim_i = 0u;
                        auto animation = new Entity();
                        entity_->push_back_child(animation);

                        auto player_srcs_i = 0;
                        for (auto& anim_json : animation_jsons)
                        {
                            std::string anim_name = anim_json["name"];
                            int frames = anim_json["frames"];
                            Anim anim_srcs;
                            for (auto i = 0; i < frames; ++i)
                                anim_srcs.push_back(static_cast<Component::Src*>(spritesheet_category[i]));
                            csu_pla_animation->add(anim_name, anim_srcs);
                        }

                        c_update_engine.add(csu_pla_move, update_group);
                        c_update_engine.add(csu_check_collision_gjk, update_group);
                        c_update_engine.add(csu_pla_camera, update_group);
                        c_update_engine.add(csu_pla_animate_move, update_group);
                        c_update_engine.add(csu_pla_animation, update_group);

                        c_render_engine.add(csr_pla_dynamic_draw, render_group);

                        if (debug)
                        {
                            std::string tileset_name = collider_json["tileset"];

                            int tileid = shape_json["circle"]["circle"];
                            // set up collider draw
                            auto& c_tset_material = *gamestate->get_child(tileset_name)->get_component<Component::Material>(0);
                            auto& c_tset_col_circle_src = *gamestate->get_child(tileset_name)->get_component<Component::Src>(tileid);

                            auto collider = new Entity();
                            auto csr_col_cam_draw = collider->add_component<Component::System::Render::CameraDraw>(c_renderer, c_tset_col_circle_src, c_pla_col_gjk_circle, c_tset_material, c_cam_position);

                            entity_->add_id_child(collider, "collider");

                            c_render_engine.add(csr_col_cam_draw, render_group);
                        }
                    }
                };
            }
		}
		
	}
}


