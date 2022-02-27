#pragma once
#include "component_trigger_input.h"
#include "component_shader.h"
#include <glm/ext/matrix_clip_space.hpp>

/*
Loads and stores Shader information for use in 

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class Shader : public Component::Trigger::IInput
			{
			public:
				using Component::Trigger::IInput::IInput;

			private:
				void create(Entity* gamestate) override final
				{
					auto& shader_json = gamestate->get_child("index")->get_component<Component::Json>(name_)->json;
					auto shader_obj = shader_json.get<nlohmann::json::object_t>();

					auto projection = glm::ortho(0.0f, (GLfloat)Game::width, (GLfloat)Game::height, 0.0f, -1.0f, 1.0f);

					// load in used shaders
					for (auto& shader : shader_obj)
					{
						auto& c_shader = *entity_->add_id_component<Component::Shader>(shader.first);
						std::string vs_file_name = shader.second[0];
						std::string fs_file_name = shader.second[1];
						c_shader.load(vs_file_name.c_str(), fs_file_name.c_str());

						// set up orthographic projection
						c_shader.use();
						c_shader.set_mat4("projection", projection);
					}
				}
			};
		}
		
	}
}
