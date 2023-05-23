#pragma once
#pragma once
#include "command.h"
#include "component_system_render_blockdraw.h"
#include <vector>

namespace Component {
	class ClearBlockDraw : public ICommand
	{
		std::vector<std::string> path_;
	
	public:

		void load(nlohmann::json json) override
		{
			path_ = json.get<std::vector<std::string>>();
		}

		void execute(Entity* gamestate) override
		{
			auto e = gamestate->get_child(*path_.begin());
			auto i = path_.begin() + 1;
			for (; i < path_.end() - 2; i++)
				e = e->get_child(*i);

			auto& csr_block_draw = *e->get_component<Component::System::Render::BlockDraw>(*i);
			csr_block_draw.clear();
		}
	};
}