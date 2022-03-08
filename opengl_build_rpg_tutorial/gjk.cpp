#include "gjk.h"

bool GJK::collide(Component::Rectangle::IGJK& col_a, Component::Rectangle::IGJK& col_b)
{
	std::array<glm::vec2, MAX_SIMPLEX> simplex;
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
			direction = col_b.get_center() - col_a.get_center();
			break;
		case 1:
			direction *= -1;
			break;
		case 2:
			b = simplex[1];
			c = simplex[0];

			auto cb = b - c;
			auto c0 = -c;

			direction = triple_product(cb, c0, cb);
			break;
		case 3:
		{
			a = simplex[2];
			b = simplex[1];
			c = simplex[0];

			auto a0 = -a;
			auto ab = b - a;
			auto ac = c - a;

			auto ab_perpindicular = triple_product(ac, ab, ab);
			auto ac_perpindicular = triple_product(ab, ac, ac);

			auto dot_ab_perpindicular = glm::dot(ab_perpindicular, a0);
			auto dot_ac_perpindicular = glm::dot(ac_perpindicular, a0);

			if (dot_ab_perpindicular > 0.0f)
			{
				simplex[0] = simplex[1]; // c = b
				simplex[1] = simplex[2]; // b = a
				i--; // a = 0
				direction = ab_perpindicular;
			}
			else if (dot_ac_perpindicular > 0.0f)
			{
				simplex[1] = simplex[2]; // b = a
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

		simplex[i] = col_b.support(direction) - col_a.support(-direction);

		if (glm::dot(simplex[i++], direction) < 0)
			return false;
	}
}

bool GJK::collide(glm::vec2 point, Component::Rectangle::IGJK& col_b)
{
	std::array<glm::vec2, MAX_SIMPLEX> simplex;

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
			direction = col_b.get_center() - point;
			break;
		case 1:
			direction *= -1;
			break;
		case 2:
			b = simplex[1];
			c = simplex[0];

			auto cb = b - c;
			auto c0 = -c;

			direction = triple_product(cb, c0, cb);
			break;
		case 3:
		{
			a = simplex[2];
			b = simplex[1];
			c = simplex[0];

			auto a0 = -a;
			auto ab = b - a;
			auto ac = c - a;

			auto ab_perpindicular = triple_product(ac, ab, ab);
			auto ac_perpindicular = triple_product(ab, ac, ac);

			auto dot_ab_perpindicular = glm::dot(ab_perpindicular, a0);
			auto dot_ac_perpindicular = glm::dot(ac_perpindicular, a0);

			if (dot_ab_perpindicular > 0.0f)
			{
				simplex[0] = simplex[1]; // c = b
				simplex[1] = simplex[2]; // b = a
				i--; // a = 0
				direction = ab_perpindicular;
			}
			else if (dot_ac_perpindicular > 0.0f)
			{
				simplex[1] = simplex[2]; // b = a
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

		simplex[i] = col_b.support(direction) - point;

		if (glm::dot(simplex[i++], direction) < 0)
			return false;
	}
}

bool GJK::AABB::collide(Component::Rectangle::IGJK& col_a, Component::Rectang& col_b)
{
	std::array<glm::vec2, MAX_SIMPLEX> simplex;
	auto i = 0;

	glm::vec2 direction, a, b, c;

	auto col_b_center = glm::vec2(col_b.x, col_b.y) - glm::vec2(col_b.w / 2.0f, col_b.h / 2.0f);
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
			direction = col_b_center - col_a.get_center();
			break;
		case 1:
			direction *= -1;
			break;
		case 2:
			b = simplex[1];
			c = simplex[0];

			auto cb = b - c;
			auto c0 = -c;

			direction = triple_product(cb, c0, cb);
			break;
		case 3:
		{
			a = simplex[2];
			b = simplex[1];
			c = simplex[0];

			auto a0 = -a;
			auto ab = b - a;
			auto ac = c - a;

			auto ab_perpindicular = triple_product(ac, ab, ab);
			auto ac_perpindicular = triple_product(ab, ac, ac);

			auto dot_ab_perpindicular = glm::dot(ab_perpindicular, a0);
			auto dot_ac_perpindicular = glm::dot(ac_perpindicular, a0);

			if (dot_ab_perpindicular > 0.0f)
			{
				simplex[0] = simplex[1]; // c = b
				simplex[1] = simplex[2]; // b = a
				i--; // a = 0
				direction = ab_perpindicular;
			}
			else if (dot_ac_perpindicular > 0.0f)
			{
				simplex[1] = simplex[2]; // b = a
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

		std::vector<glm::vec2> vertices{ glm::vec2(col_b.x, col_b.y), glm::vec2(col_b.x + col_b.w, col_b.y),
			glm::vec2(col_b.x + col_b.w, col_b.y + col_b.h), glm::vec2(col_b.x, col_b.y + col_b.h) };

		auto col_b_support = [](glm::vec2 direction, std::vector<glm::vec2> vertices, glm::vec2 center)
		{
			auto furthest_distance = std::numeric_limits<float>::lowest();
			auto furthest_vertex = glm::vec2();
			// calculate the furthest vertex along the direction
			for (auto& v : vertices)
			{
				auto distance = glm::dot(v + center, direction);
				if (distance > furthest_distance)
				{
					furthest_distance = distance;
					furthest_vertex = v + center;
				}
			}

			return furthest_vertex;
		};

		simplex[i] = col_b_support(direction, vertices, col_b_center) - col_a.support(-direction);

		if (glm::dot(simplex[i++], direction) < 0)
			return false;
	}
}
