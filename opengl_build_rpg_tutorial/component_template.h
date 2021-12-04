#pragma once
#include "component.h"

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



