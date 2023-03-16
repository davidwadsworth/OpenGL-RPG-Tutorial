#pragma once
#include "load.h"
#include "component_quadtree.h"
#include "component_json.h"

/*
List of all active colliding objects in game world.

@author David Wadsworth
*/

namespace Commmand {
	namespace Load {
		template <typename T>
		class QuadTree : public ILoad
		{
			Rect rect_;
			int max_objects_;
			std::string name_;
		public:

			void load(nlohmann::json json) override final
			{
				float x = json["rect"]["x"];
				float y = json["rect"]["y"];
				float w = json["rect"]["w"];
				float h = json["rect"]["h"];

				name_ = json["name"];
				rect_ = Rect(x, y, w, h);
				max_objects_ = json["max_objects"];
			}

			void execute(Entity* gamestate) override
			{
				auto e_quadtree = gamestate->add_id_child

				this->entity_->push_back_component<Component::QuadTree<T>>(max_objects_, rect_);
			}
		};
	}
}
