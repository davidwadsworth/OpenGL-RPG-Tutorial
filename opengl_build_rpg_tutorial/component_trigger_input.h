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
			virtual void create(EntityMap& map) = 0;
		public:

			In(std::string name)
				: entity_(nullptr), name_(name)
			{}

			void execute(EntityMap& map) override final
			{
				// if this game object is already in the map 
				if (map.find(name_) != map.end())
					map.erase(name_);

				// set up entity and add into our passed through map
				entity_ = new Entity();
				map.emplace(name_, std::unique_ptr<Entity>(entity_));

				create(map);
			}
		};
	}
}
