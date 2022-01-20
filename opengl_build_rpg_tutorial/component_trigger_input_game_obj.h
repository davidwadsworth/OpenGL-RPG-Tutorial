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
				}

				virtual void init(Entity* gamestate) = 0;
			public:
				Entity* e_game_info_ = nullptr;
				using Component::Trigger::IInput::IInput;
			};
		}
	}
}