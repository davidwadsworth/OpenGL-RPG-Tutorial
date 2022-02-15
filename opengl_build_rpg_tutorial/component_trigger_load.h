#pragma once
#include "component_trigger.h"
#include "json.hpp"

namespace Component {
	namespace Trigger
	{
		class ILoad : public Component::ITrigger
		{
		protected:
			nlohmann::json json_;
		public:

			void load(nlohmann::json json) 
			{
				json_ = json;
			}
		};
	}
}