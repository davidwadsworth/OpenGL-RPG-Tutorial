#pragma once
#include "component_system.h"


namespace Component {
	namespace System {
		class IItem : public Component::ISystem
		{
		protected:
			bool removed_;
			std::vector<Component::ISystem*> systems_;
		public:
			friend Component::Engine;
			IItem(std::vector<Component::ISystem*> systems)
				: systems_(systems), removed_(false)
			{}

			void execute() override
			{
				for (auto sys : systems_)
					sys->execute();
			}

			void remove() { removed_ = true; }
		};

	}
}