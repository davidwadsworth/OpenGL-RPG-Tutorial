#pragma once
#pragma once
#include "component_trigger.h"
#include "component_system_render_empty.h"
#include <vector>

namespace Component {
	namespace Trigger
	{
		class ClearEmpty : public Component::ITrigger
		{
			Component::System::Render::Empty& csr_empty_;
		public:
			ClearEmpty(Component::System::Render::Empty& csr_empty)
				: csr_empty_(csr_empty)
			{}

			void execute(Entity* gamestate) override
			{
				csr_empty_.clear();
			}
		};
	}
}