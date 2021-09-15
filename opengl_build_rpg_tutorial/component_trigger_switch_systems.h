#pragma once
#pragma once
#include "component_trigger.h"
#include "component_grouped_objects.h"

namespace Component {
	namespace Trigger
	{
		class SwitchSystems
		{
			Entity* game_obj_;
		public:
			SwitchSystems(Entity* game_obj)
				: game_obj_(game_obj)
			{}

			void execute(Entity* gamestate)
			{
				auto obj_render = game_obj_->get_child("render");
				auto obj_update = game_obj_->get_child("update");

				auto engine = gamestate->get_child("engine");

				auto& engine_render_systems = *engine->get_component<Component::GroupedSystems>("render");
				auto& engine_update_systems = *engine->get_component<Component::GroupedSystems>("update");

				if (game_obj_->has_component<Component::GroupedSystems>("render"))
				{
					auto& obj_render_systems = *game_obj_->get_component<Component::GroupedSystems>("render");

					for (auto i = 0; i < obj_render_systems.objects.size(); ++i)
					{
						auto vec_copy = obj_render_systems.objects[i];
						obj_render_systems.objects[i] = engine_render_systems.objects[i];
						engine_render_systems.objects[i] = vec_copy;

					}
				}

				if (game_obj_->has_component<Component::GroupedSystems>("update"))
				{
					auto& obj_update_systems = *game_obj_->get_component<Component::GroupedSystems>("update");

					for (auto i = 0; i < obj_update_systems.objects.size(); ++i) 
					{
						auto vec_copy = obj_update_systems.objects[i];
						obj_update_systems.objects[i] = engine_update_systems.objects[i];
						engine_update_systems.objects[i] = vec_copy;

					}
				}
			}
		};
	}
}