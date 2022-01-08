#pragma once
#include "component.h"

/*
Abstract class for executing updates on components and the window

@author David Wadsworth
*/

namespace Component
{
	struct ISystem : public IComponent
	{
		ISystem()
		{}

		virtual void execute() = 0;

	};
}