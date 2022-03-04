#pragma once
#include "json.hpp"

class Action
{
public:
	Action() = default;
	nlohmann::json action;
};
