#pragma once
#include "component_trigger.h"
#include "component_grouped_objects.h"
#include "component_vector.h"

namespace Component {
	namespace Trigger
	{
		class AddGameObj
		{
			Entity* game_obj_;
		public:
			AddGameObj(Entity* game_obj)
				: game_obj_(game_obj)
			{}

			void execute(Entity* gamestate)
			{
				auto engine = gamestate->get_child("engine");

				auto& engine_triggers = *engine->get_component<Component::TriggerVector>("trigger");

				if (game_obj_->has_component<Component::GroupedSystems>("render"))
				{
					auto& obj_render_systems = *game_obj_->get_component<Component::GroupedSystems>("render");

					for (auto obj_rg : obj_render_systems.objects)
						for (auto i = 0; i < obj_rg.size; ++i)
							obj_rg[i]->remove();
				}

				if (game_obj_->has_component<Component::GroupedSystems>("update"))
				{
					auto& obj_update_systems = *game_obj_->get_component<Component::GroupedSystems>("update");

					for (auto obj_ug : obj_update_systems.objects)
						for (auto i = 0; i < obj_ug.size; ++i)
							obj_ug[i]->remove();
				}

				if (game_obj_->has_component<Component::TriggerVector>("trigger remove"))
				{
					auto& obj_triggers = *game_obj_->get_component<Component::TriggerVector>("trigger remove");
					engine_triggers.insert(engine_triggers.end(), obj_triggers.begin(), obj_triggers.end());
				}
			}
		};
	}
}