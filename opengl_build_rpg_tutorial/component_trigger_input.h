#pragma once
#include "component_trigger.h"
#include "game.h"

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
			Entity* parent_;
			std::string name_; 
			virtual void create(Entity* gamestate) {}
		public:
			
			IInput(Entity* parent, std::string name)
				: entity_(nullptr), name_(name), parent_(parent)
			{}

			IInput(std::string name)
				: IInput(nullptr, name)
			{}

			void set_name(std::string name)
			{
				name_ = name;
			}

			void execute(Entity* gamestate) override final
			{
				// set up entity and add into our passed through map
				entity_ = new Entity();
				
				// if parent isn't set use gamestate
				if (!parent_)
					parent_ = gamestate;

				// if this game object is already in the parent 
				if (parent_->has_child(name_))
					parent_->remove_child(name_);

				parent_->add_id_child(entity_, name_);

				create(gamestate);
			}
		};
	}
}
