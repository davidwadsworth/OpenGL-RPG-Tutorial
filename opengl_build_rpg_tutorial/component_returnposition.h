#pragma once
#include "component.h"
#include <unordered_map>
#include <glm/glm.hpp>

namespace Component
{
	struct ReturnPosition : public IComponent, std::unordered_map<std::string, glm::vec2>
	{};
}