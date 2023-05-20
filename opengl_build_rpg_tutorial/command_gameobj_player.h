#pragma once
#include "add_component_rect.h"
#include "command.h"
#include "component_system_update_animatemove.h"
#include "component_system_update_animation.h"
#include "component_system_update_move.h"
#include "component_system_update_camera.h"
#include "component_controller_keyboard.h"
#include "component_vector.h"
#include "component_system_update_checkcollision.h"
#include "component_system_update_checkaction.h"
#include "component_system_render_cameradraw.h"
#include "component_system_update_checkactioncollision.h"
#include "component_engine.h"
#include "component_pathway.h"

/*
Creates our player object to move around in the overworld.

@author David Wadsworth
*/

namespace Command {
    namespace GameObj
    {
        class Player : public ICommand
        {
            std::string player_name_;
            std::string spritesheet_name_;

            nlohmann::json info_json_;
            nlohmann::json animation_json_;

            float scale_;
            float animation_speed_;
            float player_speed_;
            float render_group_;
            float update_group_;
            float action_distance_;

        public:
            void load(nlohmann::json json) override final
            {
                player_name_ = json["name"];
                spritesheet_name_ = json["texture"];

                info_json_ = json["info"];
                animation_json_ = json["animations"];
                
                scale_ = json["scale"];
                animation_speed_ = json["animation_speed"];
                player_speed_ = json["player_speed"];
                render_group_ = json["render_group"];
                update_group_ = json["update_group"];
                action_distance_ = json["action_distance"];
            }

            void execute(Entity* gamestate) override final
            {
                auto entity = gamestate->add_id_child(player_name_);

                auto e_spritesheet = gamestate->get_child(spritesheet_name_);

                // get renderer
                auto& c_renderer = *gamestate->get_component<Component::Renderer >("renderer");

                // get camera
                auto& c_cam_position = *gamestate->get_child("camera")->get_component<Component::Position>();

                // get action qtree
                auto& c_physicsaction_qtree = *gamestate->get_child("physics_action_qtree")->get_component<Component::PhysicsActionGJKQTree>(0);

                // get action qtree
                auto& c_action_qtree = *gamestate->get_child("action_qtree")->get_component<Component::ActionGJKQTree>(0);

                // get render engine
                auto& c_render_engine = *gamestate->get_component<Component::Engine>("render");

                // get update engine
                auto& c_update_engine = *gamestate->get_component<Component::Engine>("update");

                // get load entity
                auto e_load = gamestate->get_child("load");

                auto& c_pathway = *gamestate->get_component<Component::Pathway>("pathway");

                auto& c_pla_src = *entity->push_back_component<Component::Src>();
                auto& c_spritesheet_material = *gamestate->get_child(spritesheet_name_)->get_component<Component::Material>("material");

                auto& c_pla_col_gjk_circle = *static_cast<Component::Rectangle::IGJK*>(add_component_rect(entity, gamestate, info_json_));
                Logger::message(std::to_string(c_pla_col_gjk_circle.x) + ", " + std::to_string(c_pla_col_gjk_circle.y));
                auto csr_pla_dynamic_draw = entity->push_back_component<Component::System::Render::CameraDraw>(c_renderer, c_pla_src, c_pla_col_gjk_circle, c_spritesheet_material, c_cam_position);
                auto csu_pla_camera = entity->push_back_component<Component::System::Update::Camera>(c_pla_col_gjk_circle, c_cam_position);
                auto csu_pla_move = entity->push_back_component<Component::System::Update::Move>(c_pla_col_gjk_circle, player_speed_);

                auto csu_pla_animation = entity->push_back_component<Component::System::Update::Animation>(animation_speed_, c_pla_src);
                auto csu_pla_animate_move = entity->push_back_component<Component::System::Update::AnimateMove>(*csu_pla_animation);
                auto csu_check_collision = entity->push_back_component<Component::System::Update::CheckCollision>(c_pla_col_gjk_circle, c_physicsaction_qtree);
                auto csu_check_action_collision = entity->push_back_component<Component::System::Update::CheckActionCollision>(c_pla_col_gjk_circle, c_action_qtree, c_pathway, e_load);
                auto csu_check_action = entity->push_back_component<Component::System::Update::CheckAction>(c_pla_col_gjk_circle, c_physicsaction_qtree, e_load, c_pathway, action_distance_);

                auto anim_i = 0u;
                auto animation = new Entity();
                entity->push_back_child(animation);

                auto player_srcs_i = 0;
                for (auto& anim_json : animation_json_)
                {
                    std::string anim_name = anim_json["name"];
                    Anim anim_srcs;
                    for (std::string frame_name : anim_json["frames"])
                        anim_srcs.push_back(e_spritesheet->get_component<Component::Src>(frame_name));

                    csu_pla_animation->add(anim_name, anim_srcs);
                }

                csu_pla_animation->play("idle_down");

                c_update_engine.add(csu_pla_move, update_group_);
                c_update_engine.add(csu_check_collision, update_group_);
                c_update_engine.add(csu_check_action_collision, update_group_);
                c_update_engine.add(csu_pla_camera, update_group_);
                c_update_engine.add(csu_pla_animate_move, update_group_);
                c_update_engine.add(csu_pla_animation, update_group_);
                c_update_engine.add(csu_check_action, update_group_);

                c_render_engine.add(csr_pla_dynamic_draw, render_group_);

            }
        };
    }
}


