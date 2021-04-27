#pragma once
#include "component_trigger_input.h"
#include "component_transform.h"
#include "component_src.h"
#include "component_dest.h"
#include "component_renderer.h"
#include "component_material.h"
#include "component_system_render_camera_draw.h"
#include "component_system_update_animate_move.h"
#include "component_system_update_animation.h"
#include "component_system_update_move.h"
#include "component_system_update_camera.h"
#include "component_controller_keyboard.h"
#include "component_vector.h"
#include "component_collider_aabb.h"
#include "component_system_update_check_collision_aabb.h"

/*
Creates our player object to move around in the overworld.

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input {
			class Player : public Component::Trigger::In
			{
                GLfloat x_, y_;
			public:
				Player(std::string name, GLfloat x, GLfloat y)
					: Component::Trigger::In(name), x_(x), y_(y)
				{}

            private:
                void create(EntityMap& map) override final
                {
                    // get player shader and textures
                    auto& c_flesh_tex = *map["texture manager"]->get_component<Component::Texture>("flesh_full");
                    auto& c_sprite_shader = *map["shader manager"]->get_component<Component::Shader>("sprite");

                    // get renderer
                    auto& c_renderer = *map["renderer"]->get_component<Component::Renderer>();

                    // get controller 
                    auto& c_cont_keyboard = *map["controller"]->get_component<Component::Controller::Keyboard>();

                    // get camera
                    auto& c_cam_transform = *map["camera"]->get_component<Component::Transform>();

                    // get collision world
                    auto& c_colw_col_vec = *map["collision world"]->get_component<Component::ColliderVector>();

                    auto& c_pla_transform = *entity_->add_component<Component::Transform>(x_, y_, 64.0f);
                    auto& c_pla_src = *entity_->add_component<Component::Src>(Rect{ 0.0f, 0.0f, 64.0f, 64.0f });
                    auto& c_pla_dest = *entity_->add_component<Component::Dest>();
                    auto& c_pla_material = *entity_->add_component<Component::Material>(c_flesh_tex, c_sprite_shader, 0);
                    auto& c_pla_movement = *entity_->add_component<Component::Movement>(240.0f);
                    auto& c_pla_col_aabb = *entity_->add_component<Component::Collider::AABB>(c_pla_transform, 64.0f);

                    auto csr_pla_dynamic_draw = entity_->add_component<Component::System::Render::CameraDraw>(c_renderer, c_pla_src, c_pla_dest, c_pla_material, c_pla_transform, c_cam_transform);
                    auto csu_pla_camera = entity_->add_component<Component::System::Update::Camera>(c_pla_transform, c_cam_transform);
                    auto csu_pla_move = entity_->add_component<Component::System::Update::Move>(c_pla_transform, c_cont_keyboard, c_pla_movement);

                    auto csu_pla_animation = entity_->add_component<Component::System::Update::Animation>(4, c_pla_src);
                    auto csu_pla_animate_move = entity_->add_component<Component::System::Update::AnimateMove>(c_cont_keyboard, *csu_pla_animation);
                    auto csu_check_collision_aabb = entity_->add_component<Component::System::Update::CheckCollisionAABB>(c_pla_movement, c_pla_col_aabb, c_colw_col_vec);

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

                    auto& render_systems = *map["engine"]->get_component<Component::SystemVector>("render");
                    auto& update_systems = *map["engine"]->get_component<Component::SystemVector>("update");

                    update_systems.push_back(csu_pla_move);
                    update_systems.push_back(csu_check_collision_aabb);
                    update_systems.push_back(csu_pla_camera);
                    update_systems.push_back(csu_pla_animate_move);
                    update_systems.push_back(csu_pla_animation);

                    render_systems.push_back(csr_pla_dynamic_draw);
				}
			};
		}
		
	}
}


