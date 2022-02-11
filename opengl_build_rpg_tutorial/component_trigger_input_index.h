#pragma once
#include "component_trigger_input.h"
#include <sstream>
#include "json.hpp"
#include "component_json.h"

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class Index : public Component::Trigger::IInput
			{
				std::string path_;

				void create(Entity* gamestate) override
				{
					// load index from file
					std::stringstream indx_stream;

					try
					{
						// open files
						std::ifstream indx_file(path_);

						// read into temp string streams
						indx_stream << indx_file.rdbuf();

						// close file streams
						indx_file.close();
					}
					catch (std::exception e)
					{
						Logger::error("Failed to read index file! path = " + path_, Logger::MEDIUM);
						return;
					}

					auto indx_json = nlohmann::json::parse(indx_stream);

					auto indx_obj = indx_json.get<nlohmann::json::object_t>();

					for (auto &obj : indx_obj)
					{
						auto e_json = new Entity();
						entity_->add_id_child(e_json, obj.first);
						
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
							Logger::error("Failed to read obj file! path = " + path_, Logger::MEDIUM);
							return;
						}

						e_json->add_component<Component::Json>(nlohmann::json::parse(obj_stream));
					}

				}
			public:
				Index(std::string name, std::string path)
					: Component::Trigger::IInput(name), path_(path)
				{}
			};
		}
	}
}