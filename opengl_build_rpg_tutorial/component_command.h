#pragma once
#include "component.h"
#include "command.h"

namespace Component
{
	class CCommand : public ICommand, public IComponent
	{};
}
