#pragma once
#include "component_trigger.h"

namespace Component {
	namespace Trigger
	{
		class DeleteEntity : public ITrigger
		{
			std::string obj_id_;
		public:
			DeleteEntity(std::string obj_id)
				: obj_id_(obj_id)
			{}

			void execute(Entity* game_state) override
			{
				game_state->remove_child(obj_id_);
			}
		};
	}
}
