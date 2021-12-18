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
				Shader(std::string name)
					: Component::Trigger::IInput(name)
				{}

			private:
				void create(Entity* gamestate) override final
				{
					
					// load in used shaders
					auto vs_file_name = "resources/shaders/sprite.vs";
					auto fs_file_name = "resources/shaders/sprite.fs";

					auto& c_shader = *entity_->add_id_component<Component::Shader>("sprite");
					c_shader.load(vs_file_name, fs_file_name);

					// set up orthographic projection
					c_shader.use();
					auto projection = glm::ortho(0.0f, (GLfloat)Game::width, (GLfloat)Game::height, 0.0f, -1.0f, 1.0f);
					c_shader.set_mat4("projection", projection);

					auto fnt_vs_file_name = "resources/shaders/font.vs";
					auto fnt_fs_file_name = "resources/shaders/font.fs";

					auto& c_fnt_shader = *entity_->add_id_component<Component::Shader>("font");
					c_fnt_shader.load(fnt_vs_file_name, fnt_fs_file_name);

					c_fnt_shader.use();
					c_fnt_shader.set_mat4("projection", projection);
				}
			};
		}
		
	}
}
