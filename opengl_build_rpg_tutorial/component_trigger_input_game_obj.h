#pragma once
#include "component_trigger_input.h"
#include "component_trigger_add_game_obj.h"
#include "component_trigger_delete_game_obj.h"

namespace Component {
	namespace Trigger {
		namespace Input {
			class IGameObj : public Component::Trigger::IInput
			{
				void create(Entity* gamestate) final
				{
					e_game_info_ = new Entity();
					entity_->add_id_child(e_game_info_, "game_info");

					e_game_info_->add_id_component<Component::Trigger::AddGameObj>("add", name_);
					e_game_info_->add_id_component<Component::Trigger::RemoveGameObj>("remove", name_);
				}

				virtual void init(Entity* gamestate) = 0;
			protected:
				Entity* e_game_info_ = nullptr;
			public:
				using Component::Trigger::IInput::IInput;
			};
		}
	}
}