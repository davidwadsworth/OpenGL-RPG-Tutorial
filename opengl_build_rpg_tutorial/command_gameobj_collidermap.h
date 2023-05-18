#pragma once
#include "command.h"
#include "component_vector.h"
#include "component_system_render_offset.h"
#include "add_component_rect.h"
#include "component_engine.h"

/*
storage class for tiled collision objects

@author David Wadsworth
*/

namespace Command {
	namespace GameObj
	{
		class ColliderMap : public ICommand
		{
			float render_group_;
			std::string texture_name_, name_;
			nlohmann::json data_json_;
		public:
			void init(nlohmann::json json)
			{
				float render_group = json["render_group"];
				std::string texture_name = json["texture"]["filename"];
				data_json_ = json["data"];
				name_ = json["name"];
			}

			void execute(Entity* gamestate) override final
			{		
				auto entity = gamestate->add_id_child(name_);
				auto e_texture = gamestate->get_child(texture_name_);
				auto& c_material = *e_texture->get_component<Component::Material>("material");
				
						
				std::vector<Component::Src*> src_vec;
				std::vector<Component::Transform*> transform_vec;
				for (auto& data : data_json_)
				{
					std::string src_id = data["src"]["id"];
					auto c_src = e_texture->get_component<Component::Src>(src_id);
					src_vec.push_back(c_src);
					auto c_transform = add_component_rect(entity, gamestate, data["info"]);
					transform_vec.push_back(c_transform);
				}

				// get renderer
				auto& c_renderer = *gamestate->get_component<Component::Renderer >("renderer");

				// get camera
				auto& c_cam_position = *gamestate->get_child("camera")->get_component<Component::Position>();

				// get render engine
				auto& c_render_engine = *gamestate->get_component<Component::Engine>("render");

				auto csr_offset_render = entity->add_id_component<Component::System::Render::Offset>("render", c_cam_position, src_vec, transform_vec, c_material, c_renderer);
				c_render_engine.add(csr_offset_render, render_group_);
			}
		};
	}
}
