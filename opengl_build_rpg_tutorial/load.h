#pragma once
#include "json.hpp"
#include "command.h"

class ILoad : public ICommand
{
public:
	virtual void load(nlohmann::json json) = 0;
};