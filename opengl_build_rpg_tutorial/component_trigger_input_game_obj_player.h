#pragma once
#include "component_trigger_input_game_obj.h"
#include "component_transform.h"
#include "component_src.h"
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

/*
Creates our player object to move around in the overworld.

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
            namespace GameObj
            {
                class Player : public Component::Trigger::Input::IGameObj
                {
                    std::size_t render_group_, update_group_;
                    GLfloat x_, y_;
                public:
                    Player(std::string name, std::size_t render_group, std::size_t update_group, GLfloat x, GLfloat y)
                        : Component::Trigger::Input::IGameObj(name), render_group_(render_group), update_group_(update_group), x_(x), y_(y)
                    {}

                    void set_position(GLfloat x, GLfloat y)
                    {
                        x_ = x;
                        y_ = y;
                    }

                private:
                    void init(Entity* gamestate) override final
                    {
                        // get player shader and textures
                        auto& c_flesh_tex = *gamestate->get_child("texture manager")->get_component<Component::Texture>("flesh_full");
                        auto& c_sprite_shader = *gamestate->get_child("shader manager")->get_component<Component::Shader>("sprite");

                        // get renderer
                        auto& c_renderer = *gamestate->get_component<Component::Renderer >("renderer");

                        // get controller 
                        auto& c_cont_keyboard = *gamestate->get_child("controller")->get_component<Component::Controller::Keyboard>();

                        // get camera
                        auto& c_cam_transform = *gamestate->get_child("camera")->get_component<Component::Transform>();

                        // get collision world
                        auto& c_colw_col_vec = *gamestate->get_child("collision world")->get_component<Component::GJKVector>();

                        auto& c_pla_transform = *entity_->add_component<Component::Transform>(x_, y_, 64.0f);
                        auto& c_pla_src = *entity_->add_component<Component::Src>(Rect{ 0.0f, 0.0f, 64.0f, 64.0f });
                        auto& c_pla_material = *entity_->add_component<Component::Material>(c_flesh_tex, c_sprite_shader, 0);
                        auto& c_pla_movement = *entity_->add_component<Component::Movement>(240.0f);
                        auto& c_pla_col_gjk_circle = *entity_->add_component<Component::Collider::GJK::Circle>(c_pla_transform, 32.0f, glm::vec2(32.0f, 32.0f));

                        auto csr_pla_dynamic_draw = entity_->add_component<Component::System::Render::CameraDraw>(c_renderer, c_pla_src, c_pla_transform, c_pla_material, c_cam_transform);
                        auto csu_pla_camera = entity_->add_component<Component::System::Update::Camera>(c_pla_transform, c_cam_transform);
                        auto csu_pla_move = entity_->add_component<Component::System::Update::Move>(c_pla_transform, c_cont_keyboard, c_pla_movement);

                        auto csu_pla_animation = entity_->add_component<Component::System::Update::Animation>(4, c_pla_src);
                        auto csu_pla_animate_move = entity_->add_component<Component::System::Update::AnimateMove>(c_cont_keyboard, *csu_pla_animation);
                        auto csu_check_collision_gjk = entity_->add_component<Component::System::Update::CheckCollisionGJK>(c_pla_col_gjk_circle, c_colw_col_vec);

                        // set up collider draw
                        auto& c_tset_material = *gamestate->get_child("TestTilesetGJK")->get_component<Component::Material>(0);
                        auto& c_tset_col_circle_src = *gamestate->get_child("TestTilesetGJK")->get_component<Component::Src>(71);

                        auto collider = new Entity();
                        auto csr_col_cam_draw = collider->add_component<Component::System::Render::CameraDraw>(c_renderer, c_tset_col_circle_src, c_pla_transform, c_tset_material, c_cam_transform);

                        entity_->add_id_child(collider, "collider");

                        // set up flesh animations
                        std::string anims[] = {
                            "idle down",
                            "idle up",
                            "idle right",
                            "idle left",
                            "walk down",
                            "walk up",
                            "walk right",
                            "walk left"
                        };

                        auto anim_i = 0u;
                        auto animation = new Entity();
                        entity_->push_back_child(animation);

                        auto flesh_tex_cols = c_flesh_tex.width / 64u;

                        for (auto i = 0; i < 4; ++i)
                        {
                            Rect rect{
                                (GLfloat)(anim_i % flesh_tex_cols) * 64.0f,
                                (GLfloat)(anim_i / flesh_tex_cols) * 64.0f,
                                64.0f,
                                64.0f
                            };

                            auto idle = animation->push_back_component<Component::Src>(rect);
                            csu_pla_animation->add(anims[anim_i++], Anim{ idle });
                        }

                        for (auto i = 0; i < 4; ++i)
                        {
                            Rect rect{
                                (GLfloat)((anim_i + i) % flesh_tex_cols) * 64.0f,
                                (GLfloat)((anim_i + i) / flesh_tex_cols) * 64.0f,
                                64.0f,
                                64.0f
                            };

                            Rect rect2{
                                (GLfloat)((anim_i + i + 1) % flesh_tex_cols) * 64.0f,
                                (GLfloat)((anim_i + i + 1) / flesh_tex_cols) * 64.0f,
                                64.0f,
                                64.0f
                            };

                            auto walk_1 = animation->push_back_component<Component::Src>(rect);
                            auto walk_2 = animation->push_back_component<Component::Src>(rect2);
                            csu_pla_animation->add(anims[anim_i++], Anim{ walk_1, walk_2 });
                        }

                        // get render and update systems
                        auto& c_player_render_systems = *e_game_info_->add_id_component<Component::GroupedSystems>("render");
                        auto& c_player_update_systems = *e_game_info_->add_id_component<Component::GroupedSystems>("update");

                        c_player_update_systems.add(csu_pla_move, update_group_);
                        c_player_update_systems.add(csu_check_collision_gjk, update_group_);
                        c_player_update_systems.add(csu_pla_camera, update_group_);
                        c_player_update_systems.add(csu_pla_animate_move, update_group_);
                        c_player_update_systems.add(csu_pla_animation, update_group_);

                        c_player_render_systems.add(csr_pla_dynamic_draw, render_group_);
                        c_player_render_systems.add(csr_col_cam_draw, render_group_);
                    }
                };
            }
		}
		
	}
}


