#pragma once
#include "component.h"

/*
	abstract class for executing updates on components and the window
	@author David Wadsworth
*/

namespace Component
{
	struct System : Comp
	{
		virtual void execute() = 0;
	};
}