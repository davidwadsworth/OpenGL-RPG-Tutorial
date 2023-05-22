#pragma once
#include "command.h"
#include "component_texture.h"
#include "component_json.h"
#include "delimiter_split.h"

/*
Loads and stores Textures for overworld

@author David Wadsworth
*/

namespace Command {
	namespace GameObj {
		class Texture : public ICommand
		{
			std::string name_;
			std::vector<std::string> tex_files_;
		public:
			void load(nlohmann::json json) override final
			{
				name_ = json["name"];
				tex_files_ = json["texfiles"].get<std::vector<std::string>>();
			}

			void execute(Entity* gamestate) override final
			{
				auto e_texture = gamestate->add_id_child(name_);

				for (std::string tex_file : tex_files_)
				{
					auto tex_name = delimiter_split(delimiter_split(tex_file.c_str(), '/').back().c_str(), '.')[0];
					auto& c_texture = *e_texture->add_id_component<Component::Texture>(tex_name);
					tex_file = "Resources/Images/" + tex_file;
					c_texture.load(tex_file.c_str());
				}
			}
		};
	}
}
