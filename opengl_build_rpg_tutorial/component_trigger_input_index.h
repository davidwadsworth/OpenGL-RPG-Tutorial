#pragma once
#include "component_trigger_input.h"
#include <sstream>
#include "json.hpp"
#include "component_json.h"

constexpr auto INDEX_PATH = "Resources/Data/index.json";

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class Index : public Component::Trigger::IInput
			{
				void create(Entity* gamestate) override
				{
					// load index from file
					std::stringstream indx_stream;

					try
					{
						// open files
						std::ifstream indx_file(INDEX_PATH);

						// read into temp string streams
						indx_stream << indx_file.rdbuf();

						// close file streams
						indx_file.close();
					}
					catch (std::exception e) {
						Logger::error("Failed to read index file! path = " + std::string(INDEX_PATH), Logger::HIGH);
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

						entity_->add_id_component<Component::Json>(obj.first, nlohmann::json::parse(obj_stream));
					}

				}
			public:
				using Component::Trigger::IInput::IInput;
			};
		}
	}
}