#pragma once

#include "component.h"
#include "json.hpp"

namespace Component{
	class State : public IComponent
	{
	public:
		nlohmann::json previous_state;
		nlohmann::json current_state;
	};

}
