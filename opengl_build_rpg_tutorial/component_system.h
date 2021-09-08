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
			: remove_(false)
		{}

		virtual void execute() = 0;

		void remove() { remove_ = true; }
		bool is_removed() { return remove_; }
	private:
		bool remove_;
	};
}