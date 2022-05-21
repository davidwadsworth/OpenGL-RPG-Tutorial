#pragma once
#include "component.h"

namespace Component
{
	class Navigator : public IComponent
	{
	public:
		virtual int execute() = 0;
	};
}

