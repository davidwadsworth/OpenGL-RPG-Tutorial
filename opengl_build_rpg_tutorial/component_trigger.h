#pragma once
#include "component.h"
#include "entity.h"
#include "command.h"

/*
Systems whoes whole purpose is to execute once and be unused until needed again

@author David Wadsworth
*/

namespace Component
{
	struct ITrigger : public IComponent, public ICommand
	{};
}