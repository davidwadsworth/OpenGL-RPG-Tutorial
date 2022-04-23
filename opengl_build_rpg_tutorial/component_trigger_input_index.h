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
				std::string filename_;

				void create(Entity* gamestate) override
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

					auto indx_json = nlohmann::json::parse(indx_stream);

					auto indx_obj = indx_json.get<nlohmann::json::object_t>();

					for (auto &obj : indx_obj)
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

						entity_->add_id_component<Component::Json>(obj.first, nlohmann::json::parse(obj_stream));
					}

				}
			public:
				Index(std::string name, std::string filename)
					: Component::Trigger::IInput(name), filename_(filename)
				{}
			};
		}
	}
}