#pragma once
#include "command.h"
#include <sstream>
#include "json.hpp"
#include "component_json.h"

namespace Command {
	namespace GameObj {
		class Index : public ICommand
		{
			std::string filename_, name_;
		public:

			void load(nlohmann::json json) override
			{
				filename_ = json["load"];
				name_ = json["name"];
			}

			void execute(Entity* gamestate) override
			{
				// load index from file
				std::stringstream indx_stream;

				try
				{
					// open files
					std::ifstream indx_file("Resources/Data/" + filename_);

					// read into temp string streams
					indx_stream << indx_file.rdbuf();

					// close file streams
					indx_file.close();
				}
				catch (std::exception e) {
					Logger::error("Failed to read index file! path = " + std::string("Resources/Data/") + filename_, Logger::HIGH);
				}
				auto e_index = gamestate->add_id_child(name_);
				auto indx_json = nlohmann::json::parse(indx_stream);

				auto indx_obj = indx_json.get<nlohmann::json::object_t>();

				for (auto& obj : indx_obj)
				{
					// load index from file
					std::stringstream obj_stream;

					std::string obj_path = obj.second;

					try
					{
						// open files
						std::ifstream obj_file(obj_path);

						// read into temp string streams
						obj_stream << obj_file.rdbuf();

						// close file streams
						obj_file.close();
					}
					catch (std::exception e)
					{
						Logger::error("Failed to read obj file! path = " + obj_path, Logger::LOW);
						continue;
					}

					if (!obj_stream)
						Logger::error(std::string("invalid json: path = ") + obj_path, Logger::HIGH);

					e_index->add_id_component<Component::Json>(obj.first, nlohmann::json::parse(obj_stream));
				}

			}
		};
	}
}
		