#pragma once
#include "component.h"
#include "entity.h"
#include "component_vector.h"
#include "json.hpp"
#include "component_trigger_load.h"
#include "component_json.h"

namespace Component
{
	class ParseAction : public IComponent
	{
		Entity* e_loadcache_;
		Component::TriggerVector& c_triggervec_;
	public:
		ParseAction(Entity* e_loadcache, Component::TriggerVector& c_triggervec)
			: e_loadcache_(e_loadcache), c_triggervec_(c_triggervec)
		{}

		void parse(nlohmann::json data_json)
		{
			for (auto action : data_json["action"])
			{
				std::string load = action["load"];
				auto action_data_json = action["data"];
				auto external_json = action["external"];

				if (action_data_json.contains("internal"))
					for (std::string internal: action_data_json["internal"])
						external_json.push_back(data_json[internal]);
										 
				auto ct_load = e_loadcache_->get_component<Component::Trigger::ILoad>(load);
				ct_load->load(external_json);

				c_triggervec_.push_back(ct_load);
			}
		}
	};
}