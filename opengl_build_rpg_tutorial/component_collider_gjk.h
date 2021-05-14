#pragma once
#include "component_collider.h"

namespace Component {
	namespace Collider
	{
		class IGJK : public Component::ICollider
		{
		protected:
			std::array<glm::vec2, MAX_SIMPLEX> simplex_;
		public:
			using Component::ICollider::ICollider;
			virtual glm::vec2 support(glm::vec2 direction) = 0;
			virtual bool collide(Component::Collider::IGJK& col);
		};
	}
}

