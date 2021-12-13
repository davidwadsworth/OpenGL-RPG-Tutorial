#pragma once
#include "component_trigger.h"
#include "component_grouped_objects.h"
#include "component_vector.h"

namespace Component {
	namespace Trigger
	{
		class AddGameObj : public Component::ITrigger
		{
			Entity* game_obj_;
		public:
			AddGameObj(Entity* game_obj)
				: game_obj_(game_obj)
			{}

			void execute(Entity* gamestate) override
			{
				auto engine = gamestate->get_child("engine");

				auto& engine_render_systems = *engine->get_component<Component::GroupedSystems>("render");
				auto& engine_update_systems = *engine->get_component<Component::GroupedSystems>("update");
				auto& engine_triggers = *engine->get_component<Component::TriggerVector>("trigger");

				if (game_obj_->has_component<Component::GroupedSystems>("render"))
				{
					auto& obj_render_systems = *game_obj_->get_component<Component::GroupedSystems>("render");

					for (auto i = 0; i < obj_render_systems.objects.size(); ++i)
						for (auto j = 0; j < obj_render_systems.objects[i].size; ++j)
							engine_render_systems.add(obj_render_systems.objects[i][j], i);
				}

				if (game_obj_->has_component<Component::GroupedSystems>("update"))
				{
					auto& obj_update_systems = *game_obj_->get_component<Component::GroupedSystems>("update");

					for (auto i = 0; i < obj_update_systems.objects.size(); ++i)
						for (auto j = 0; j < obj_update_systems.objects[i].size; ++j)
							engine_update_systems.add(obj_update_systems.objects[i][j], i);
				}

				if (game_obj_->has_component<Component::TriggerVector>("trigger add"))
				{
					auto& obj_triggers = *game_obj_->get_component<Component::TriggerVector>("trigger add");
					engine_triggers.insert(engine_triggers.end(), obj_triggers.begin(), obj_triggers.end());
				}
			}
		};
	}
}