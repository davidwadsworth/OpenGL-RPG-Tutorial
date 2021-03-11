#pragma once
#include "component.h"

/*
Abstract class for executing updates on components and the window

@author David Wadsworth
*/

namespace Component
{
	struct System : public Comp
	{
		virtual void execute() = 0;
	};
}