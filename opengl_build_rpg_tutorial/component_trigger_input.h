#pragma once
#include "component_trigger.h"

/*
Handles all game object creation related operations, generally from data classes. And adds the game object into the executed map

@author David Wadsworth
*/

namespace Component {
	namespace Trigger {
		class IInput : public Component::ITrigger
		{
		protected:
			Entity* entity_;


		private:
			virtual void create(Entity* gamestate) = 0;
		public:
			std::string name_;
			IInput(std::string name)
				: entity_(nullptr), name_(name)
			{}

			Entity* operator->()
			{
				return entity_;
			}

			void execute(Entity* gamestate) override
			{
				// set up entity and add into our passed through map
				entity_ = new Entity();
				
				// if this game object is already in the gamestate 
				if (gamestate->has_child(name_))
					gamestate->remove_child(name_);

				gamestate->add_id_child(entity_, name_);

				create(gamestate);
			}

			void execute(Entity* gamestate, Entity* parent)
			{
				// set up entity and add into our passed through map
				entity_ = new Entity();

				// if this game object is already in the parent 
				if (parent->has_child(name_))
					parent->remove_child(name_);

				parent->add_id_child(entity_, name_);

				create(gamestate);
			}
		};
	}
}
