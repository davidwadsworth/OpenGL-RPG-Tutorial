#pragma once
#include "component.h"
#include <array>

constexpr auto MAX_KEYS = 1024u;

namespace Component
{
	template <typename T, std::size_t sz>
	class Array : public IComponent, public std::array<T, sz>
	{};
#define KeyboardArray Array<bool, MAX_KEYS>
}