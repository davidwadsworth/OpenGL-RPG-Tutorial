#pragma once
#include "command.h"
#include "component_shader.h"
#include <glm/ext/matrix_clip_space.hpp>
#include "component_json.h"
#include "game.h"

/*
Loads and stores Shader information for use in 

@author David Wadsworth
*/

namespace Command {
	namespace GameObj {
		class Shader : public ICommand
		{
			nlohmann::json shader_json_;
			std::string name_;
		public:

			void load(nlohmann::json json) override final
			{
				shader_json_ = json["load"];
				name_ = json["name"];
			}

			void execute(Entity* gamestate) override final
			{
				auto e_shader = gamestate->add_id_child(name_);

				auto projection = glm::ortho(0.0f, (GLfloat)Game::width, (GLfloat)Game::height, 0.0f, -1.0f, 1.0f);

				// load in used shaders
				for (auto& shader : shader_json_)
				{
					std::string shader_name = shader["name"];
					auto& c_shader = *e_shader->add_id_component<Component::Shader>(shader_name);
					std::string vs_file_name = shader["vs"];
					vs_file_name = "Resources/Shaders/" + vs_file_name;
					std::string fs_file_name = shader["fs"];
					fs_file_name = "Resources/Shaders/" + fs_file_name;
					c_shader.load(vs_file_name.c_str(), fs_file_name.c_str());

					// set up orthographic projection
					c_shader.use();
					c_shader.set_mat4("projection", projection);
				}
			}
		};
	}
}
