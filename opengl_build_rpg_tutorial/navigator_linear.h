#pragma once
#include "navigator.h"

namespace Navigator
{
	class Linear : public INavigator
	{
	public:
		void init(Entity* e_gamestate) override
		{}

		int navigate() override
		{
			return 1;
		}
	};
}