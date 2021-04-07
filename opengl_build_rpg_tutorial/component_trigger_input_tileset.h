#pragma once
#include "component_trigger_input.h"

namespace Component {
	namespace Trigger {
		namespace Input
		{
			class TileSet : public Component::Trigger::In
			{
				std::string path_;
			public:
				TileSet(std::string name, std::string path)
					: Component::Trigger::In(name), path_(path)
				{}

			private:
				void create(EntityMap& map) override final
				{
					//TODO
				}
			};
		}
	}
}