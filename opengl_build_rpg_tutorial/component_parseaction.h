#pragma once
#include "component.h"
#include "entity.h"
#include "component_vector.h"
#include "json.hpp"
#include "component_trigger_load.h"
#include "component_json.h"

namespace Component
{
	class ParseAction
	{
		Entity* e_loadcache_,* e_index_;
		Component::TriggerVector& c_triggervec_;
	public:
		ParseAction(Entity* e_loadcache, Entity* e_index, Component::TriggerVector& c_triggervec)
			: e_loadcache_(e_loadcache),e_index_(e_index), c_triggervec_(c_triggervec)
		{}

		void parse(nlohmann::json data_json)
		{
			for (auto action : data_json)
			{
				std::string load = action["load"];
				std::string name = action["filename"];
				int pos = action["pos"];

				auto load_json = e_index_->get_component<Component::Json>(name)->json[pos];

				auto ct_load = e_loadcache_->get_child(load)->get_component<Component::Trigger::ILoad>(0);
				ct_load->load(load_json);

				c_triggervec_.push_back(ct_load);
			}
		}
	};
}