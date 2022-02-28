#pragma once
#include "component.h"

/*
Storage component for generics (int, float, etc..)

@author David Wadsworth
*/

namespace Component
{

	template <typename T>
	class Template : public IComponent
	{
	public:
		Template(T value)
			: value(value)
		{}

		T value;
	};

#define Integer Template<int>
}



