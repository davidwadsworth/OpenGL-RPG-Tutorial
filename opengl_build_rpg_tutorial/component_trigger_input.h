#pragma once
#include "component_trigger.h"
#include "game.h"

/*
Handles all game object creation related operations, generally from data classes. And adds the game object into the executed map

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		class In : public Component::Trig
		{
		protected:
			Entity* entity_;
		private:
			std::string name_;
			virtual void create(Entity* gamestate) = 0;
		public:

			In(std::string name)
				: entity_(nullptr), name_(name)
			{}

			void execute(Entity* gamestate) override final
			{
				// if this game object is already in the map 
				if (gamestate->has_child(name_))
					gamestate->remove_child(name_);

				// set up entity and add into our passed through map
				entity_ = new Entity();
				gamestate->add_id_child(entity_, name_);

				create(gamestate);
			}
		};
	}
}
