#pragma once
#include "component_trigger_input.h"
#include "component_shader.h"

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class Shader : public Component::Trigger::In
			{
			public:
				Shader(std::string name)
					: Component::Trigger::In(name)
				{}

			private:
				void create(EntityMap& map) override final
				{
					// load in used shaders
					auto vs_file_name = "resources/shaders/sprite.vs";
					auto fs_file_name = "resources/shaders/sprite.fs";

					auto& c_shader = *entity_->add_component_str_id<Component::Shader>("sprite");
					c_shader.load(vs_file_name, fs_file_name);
				}
			};
		}
		
	}
}
