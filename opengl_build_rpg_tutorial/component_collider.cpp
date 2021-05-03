#include "component_collider.h"
#include <glm/glm.hpp>

bool Component::Col::collide(Component::Col& col)
{
	auto i = 0;

	glm::vec2 direction, a, b, c;

	auto triple_product = [](glm::vec2 a, glm::vec2 b, glm::vec2 c)
	{
		auto a3 = glm::vec3(a, 0);
		auto b3 = glm::vec3(b, 0);
		auto c3 = glm::vec3(c, 0);

		return glm::vec2(glm::cross(glm::cross(a3, b3), c3));
	};

	while (true)
	{
		switch (i)
		{
		case 0:
			direction = this->get_center() - col.get_center();
			break;
		case 1:
			direction *= -1;
			break;
		case 2:
			b = simplex_[1];
			c = simplex_[0];

			auto cb = b - c;
			auto c0 = -c;

			direction = triple_product(cb, c0, cb);
			break;
		case 3:
		{
			a = simplex_[2];
			b = simplex_[1];
			c = simplex_[0];

			auto a0 = -a;
			auto ab = b - a;
			auto ac = c - a;

			auto ab_perpindicular = triple_product(ac, ab, ab);
			auto ac_perpindicular = triple_product(ab, ac, ac);

			auto dot_ab_perpindicular = glm::dot(ab_perpindicular, a0);
			auto dot_ac_perpindicular = glm::dot(ac_perpindicular, a0);

			if (dot_ab_perpindicular > 0.0f)
			{
				simplex_[0] = simplex_[1]; // c = b
				simplex_[1] = simplex_[2]; // b = a
				i--; // a = 0
				direction = ab_perpindicular;
			}
			else if (dot_ac_perpindicular > 0.0f)
			{
				simplex_[1] = simplex_[2]; // b = a
				i--; // a = 0
				direction = ac_perpindicular;
			}
			else
				return true;
			break;
		}
		default:
			return false;
			break;
		}

		simplex_[i] = this->support(direction) - col.support(-direction);

		if (glm::dot(simplex_[i++], direction) < 0)
			return false;
	}
}

