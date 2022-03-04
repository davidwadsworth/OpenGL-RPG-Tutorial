#pragma once
#include "add_component_rect.h"
#include "component_system_update_animatemove.h"
#include "component_system_update_animation.h"
#include "component_system_update_move.h"
#include "component_system_update_camera.h"
#include "component_controller_keyboard.h"
#include "component_vector.h"
#include "component_system_update_checkcollision.h"
#include "component_system_update_checkaction.h"
#include "component_system_render_cameradraw.h"

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
                        auto player_name = delimiter_split(name_.c_str(), '_')[0];

                        auto& player_json = gamestate->get_child("index")->get_component<Component::Json>(name_)->json;

                        float scale = player_json["scale"];

                        std::string spritesheet_name = player_json["spritesheet"]["filename"];
                        std::string spritesheet_category_name = player_json["spritesheet"]["groupid"];

                        auto info_json = player_json["info"];

                        std::vector<nlohmann::json> animation_jsons = player_json["animation"];

                        float animation_speed = player_json["animation_speed"];
                        float player_speed = player_json["player_speed"];
                        float render_group = player_json["render_group"];
                        float update_group = player_json["update_group"];
                        float action_distance = player_json["action_distance"];

                        auto spritesheet_category = gamestate->get_child(spritesheet_name)->get_child(spritesheet_category_name)->get_component_list<Component::Src>();

                        auto c_idle_down_src = dynamic_cast<Rect*>(spritesheet_category[0]);

                        // get renderer
                        auto& c_renderer = *gamestate->get_component<Component::Renderer >("renderer");

                        // get controller 
                        auto& c_cont_keyboard = *gamestate->get_child("controller")->get_component<Component::Controller::Keyboard>();

                        // get camera
                        auto& c_cam_position = *gamestate->get_child("camera")->get_component<Component::Position>();

                        // get collision qtree
                        auto& c_colw_col_qtree = *gamestate->get_child("collision_qtree")->get_component<Component::QuadTree>(0);

                        // get action qtree
                        auto& c_action_qtree = *gamestate->get_child("action_qtree")->get_component<Component::QuadTree>(0);

                        // get render engine
                        auto& c_render_engine = *gamestate->get_component<Component::Engine>("render_engine");

                        // get update engine
                        auto& c_update_engine = *gamestate->get_component<Component::Engine>("update_engine");

                        // get parse action
                        auto& c_parse_action = *gamestate->get_child("load")->get_component<Component::ParseAction>("parse");

                        auto& c_pla_src = *entity_->push_back_component<Component::Src>(*c_idle_down_src);
                        auto& c_spritesheet_material = *gamestate->get_child(spritesheet_name)->get_component<Component::Material>("material");

                        auto& c_pla_col_gjk_circle = *static_cast<Component::Collider::IGJK*>(add_component_rect(entity_, gamestate, info_json));
                        auto csr_pla_dynamic_draw = entity_->push_back_component<Component::System::Render::CameraDraw>(c_renderer, c_pla_src, c_pla_col_gjk_circle, c_spritesheet_material, c_cam_position);
                        auto csu_pla_camera = entity_->push_back_component<Component::System::Update::Camera>(c_pla_col_gjk_circle, c_cam_position);
                        auto csu_pla_move = entity_->push_back_component<Component::System::Update::Move>(c_pla_col_gjk_circle, c_cont_keyboard, player_speed);

                        auto csu_pla_animation = entity_->push_back_component<Component::System::Update::Animation>(animation_speed, c_pla_src);
                        auto csu_pla_animate_move = entity_->push_back_component<Component::System::Update::AnimateMove>(c_cont_keyboard, *csu_pla_animation);
                        auto csu_check_collision = entity_->push_back_component<Component::System::Update::CheckCollision>(c_pla_col_gjk_circle, c_colw_col_qtree);
                        auto csu_check_action = entity_->push_back_component<Component::System::Update::CheckAction>(c_cont_keyboard, c_pla_col_gjk_circle, c_action_qtree, c_parse_action, action_distance);

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
                        c_update_engine.add(csu_check_collision, update_group);
                        c_update_engine.add(csu_pla_camera, update_group);
                        c_update_engine.add(csu_pla_animate_move, update_group);
                        c_update_engine.add(csu_pla_animation, update_group);

                        c_render_engine.add(csr_pla_dynamic_draw, render_group);

                    }
                public:
                    using Component::Trigger::IInput::IInput;
                };
            }
		}
		
	}
}


