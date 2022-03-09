#pragma once
#include "component.h"
#include "json.hpp"

/*
TODO

@author David Wadsworth
*/

namespace Component
{
	class Json : public IComponent {
	public:
		Json(nlohmann::json json)
			: json(json)
		{}

		nlohmann::json json;
	};
}