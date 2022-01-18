#pragma once
#include "component_system.h"


namespace Component {
	namespace System {
		class IItem : public Component::ISystem
		{
		protected:
			friend class Component::Engine;
			std::vector<Component::ISystem*> systems_;
		public:
			IItem(std::vector<Component::ISystem*> systems)
				: systems_(systems)
			{}

			void execute() override
			{
				for (auto sys : systems_)
					sys->execute();
			}
		};
	}
}