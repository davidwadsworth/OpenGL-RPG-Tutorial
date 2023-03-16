#pragma once
#include "entity.h"


class ICommand
{
public:
	virtual void execute(Entity* e_gamestate) = 0;
};