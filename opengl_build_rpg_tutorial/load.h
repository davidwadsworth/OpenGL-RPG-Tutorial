#pragma once
#include "json.hpp"

class ILoad
{
public:
	virtual void load(nlohmann::json json, Entity* e_gamestate) = 0;
};