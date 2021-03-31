#pragma once
#include "component.h"
#include <vector>

namespace Component
{
	template <typename T>
	struct Vector : public Comp, public std::vector<T> {};
}