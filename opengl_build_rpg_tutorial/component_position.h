#pragma once
#include "component.h"
#include <glm/vec2.hpp>

namespace Component
{
	class Position : public IComponent, public glm::vec2 {};
}