#pragma once
#include "component_system_item.h"
#include <vector>

namespace Component {
	namespace System {
		namespace Item
		{
			class Empty : public Component::System::IItem
			{
				std::size_t draw_calls_;
			public:
				Empty(std::vector<Component::ISystem*> systems)
					: Component::System::IItem(systems), draw_calls_(0)
				{}

				void set_draw_calls(std::size_t draw_calls) { draw_calls_ = draw_calls; }

				void execute() override final
				{
					for (auto i = 0ull; i < draw_calls_; ++i)
						systems_[i]->execute();
				}
			};
		}
	}
}

