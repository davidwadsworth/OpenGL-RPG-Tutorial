#pragma once
#include "component_trigger.h"
#include <string>


namespace Component {
	class MessageHub;
	namespace Trigger
	{
		class IService : public Component::ITrigger
		{
			friend class Component::MessageHub;
			virtual void message(std::string msg) = 0;
		public:
		};
	}
}