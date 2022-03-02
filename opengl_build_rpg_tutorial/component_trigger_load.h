#pragma once
#include "component_trigger.h"
#include "json.hpp"

namespace Component {
	namespace Trigger
	{
		class ILoad : public Component::ITrigger
		{
		public:

			virtual void load(nlohmann::json json) = 0;
		};
	}
}