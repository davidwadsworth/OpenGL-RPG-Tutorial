#pragma once
#include "component_trigger.h"
#include "component_grouped_objects.h"

namespace Component {
	namespace Trigger
	{
		class CleanEngine
		{
		public:
			void execute(Entity* gamestate)
			{
				auto engine = gamestate->get_child("engine");
				auto grouped_render_systems = engine->get_component<Component::GroupedSystems>("render");
				auto grouped_update_systems = engine->get_component<Component::GroupedSystems>("update");

				for (auto grs : grouped_render_systems->objects)
					for (auto i = 0; i < grs.size; ++i)
						if (grs[i]->is_removed())
							grs.remove(i);

				for (auto gus : grouped_update_systems->objects)
					for (auto i = 0; i < gus.size; ++i)
						if (gus[i]->is_removed())
							gus.remove(i);
			}
		};
	}
}