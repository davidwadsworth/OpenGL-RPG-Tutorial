#pragma once
#pragma once
#include "component_trigger.h"
#include "component_system_render_blockdraw.h"
#include <vector>

namespace Component {
	namespace Trigger
	{
		class ClearBlockDraw : public Component::ITrigger
		{
			Component::System::Render::BlockDraw& csr_block_draw_;
		public:
			ClearBlockDraw(Component::System::Render::BlockDraw& csr_block_draw)
				: csr_block_draw_(csr_block_draw)
			{}

			void execute(Entity* gamestate) override
			{
				csr_block_draw_.clear();
			}
		};
	}
}