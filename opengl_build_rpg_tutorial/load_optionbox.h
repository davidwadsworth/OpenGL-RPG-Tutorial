#pragma once
#include "component_trigger_load.h"


namespace Load
{
	class OptionBox : public Component::Trigger::ILoad
	{
		std::string alignment_;
		nlohmann::json cursor_json_;
		float padding_;
	public:

		void load(nlohmann::json json)
		{

		}

		void execute(Entity* gamestate) override
		{
		}
	};
}