#pragma once
#include "component.h"
#include "game.h"

/*
Systems whoes whole purpose is to execute once and be unused until needed again

@author David Wadsworth
*/

namespace Component
{
	struct ITrigger : public IComponent
	{
		virtual void execute(Entity* gamestate) = 0;
	};
}