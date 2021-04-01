#pragma once
#include "component.h"
#include "game.h"

namespace Component
{
	struct Trig : public Comp
	{
		virtual void execute(EntityMap& map) = 0;
	};
}