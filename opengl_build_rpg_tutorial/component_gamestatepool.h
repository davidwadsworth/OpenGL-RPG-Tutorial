#pragma once
#include "component.h"
#include <unordered_map>

namespace Component
{
	struct PoolObj
	{
		std::string name;
		char touches = 0;
	};

	struct GameStatePool : public IComponent, std::unordered_map<std::string, PoolObj>
	{};
}

