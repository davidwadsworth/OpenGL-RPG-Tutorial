#pragma once
#include "entity.h"
#include "json.hpp"


class ICommand
{
public:
	virtual void load(nlohmann::json json) {}
	virtual void execute(Entity* e_gamestate) = 0;
}; 