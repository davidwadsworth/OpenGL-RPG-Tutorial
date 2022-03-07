#pragma once
#include "component_rect_gjk.h"
#include "component_rect_gjk_physics.h"
#include <glm/glm.hpp>
constexpr auto MAX_SIMPLEX = 3;
/*
Calculate collisions by finding a triangle which contains the origin point (0, 0)
between the Minkowski Difference of two colliders

Minkowski Difference:
	plotting all possible combinations of adding points of A with points of (-B).
Then finding the farthest edge points that create a convex polygon (Convex hull)

void collide(Component::Collider::IGJK& col):
	For this particular implementation of GJK we subvert creating the full minkowski difference by instead
using best guess points from A - the best guess points from B and if they are close to forming a triangle
over the origin repeating the guesses until out of possible guess points or enclosing the origin (0, 0) with a triangle

glm::vec2[3] simplex_:
	array of best guess A - B points;

glm::vec2 support(glm::vec2 direction):
	calculate the closest edge point of the collider within the direction provided

@author David Wadsworth
*/
namespace GJK
{
	static bool collide(Component::Rectangle::IGJK& col_a, Component::Rectangle::IGJK& col_b)
	{
		static std::array<glm::vec2, MAX_SIMPLEX> simplex;
		auto i = 0;

		glm::vec2 direction, a, b, c;

		static auto triple_product = [](glm::vec2 a, glm::vec2 b, glm::vec2 c)
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
}