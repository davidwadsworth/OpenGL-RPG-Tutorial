#pragma once
#include "component_rect.h"
#include "rect.h"
#include "action.h"

namespace Component {
	namespace Rectangle 
	{
		class Action : public Component::Rectang, public Act
		{
		public:
			using Component::Rectang::Rectang;
		};
	}
}