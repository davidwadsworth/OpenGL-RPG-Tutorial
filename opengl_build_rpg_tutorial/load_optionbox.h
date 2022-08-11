#pragma once
#include "load.h"


namespace Load
{
	class OptionBox : public ILoad
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