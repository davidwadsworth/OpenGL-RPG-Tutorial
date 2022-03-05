#pragma once
#include "component_trigger_input.h"
#include "component_vector.h"
#include "component_system_render_offset.h"
#include "add_component_rect.h"

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
						auto& collidermap_json = gamestate->get_child("index")->get_component<Component::Json>(name_)->json;
						float render_group = collidermap_json["render_group"];
						std::string texture_name = collidermap_json["texture"]["filename"];
						
						auto e_texture = gamestate->get_child(texture_name);
						auto& c_material = *e_texture->get_component<Component::Material>("material");
						auto& data_json = collidermap_json["data"];
						
						std::vector<Component::Src*> src_vec;
						std::vector<Component::Transform*> transform_vec;
						for (auto& data : data_json)
						{
							std::string src_id = data["src"]["id"];
							auto c_src = e_texture->get_component<Component::Src>(src_id);
							src_vec.push_back(c_src);
							auto c_transform = add_component_rect(entity_, gamestate, data["info"]);
							transform_vec.push_back(c_transform);
						}

						// get renderer
						auto& c_renderer = *gamestate->get_component<Component::Renderer >("renderer");

						// get camera
						auto& c_cam_position = *gamestate->get_child("camera")->get_component<Component::Position>();

						// get render engine
						auto& c_render_engine = *gamestate->get_component<Component::Engine>("render");

						auto csr_offset_render = entity_->add_id_component<Component::System::Render::Offset>("render", c_cam_position, src_vec, transform_vec, c_material, c_renderer);
						c_render_engine.add(csr_offset_render, render_group);
					}
				public:
					using Component::Trigger::IInput::IInput;
				};
			}
		}
	}
}
