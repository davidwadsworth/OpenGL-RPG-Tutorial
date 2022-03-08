#include "aabb.h"

bool AABB::collide(Component::Rectang& rect_a, Component::Rectang& rect_b)
{
	return (rect_a.x < rect_b.x + rect_b.w && rect_a.x + rect_a.w > rect_b.x) &&
		(rect_a.y < rect_b.y + rect_b.h && rect_a.y + rect_a.h > rect_b.y);
}

bool AABB::collide(glm::vec2 point, Component::Rectang& rect_b)
{
	return (point.x < rect_b.x + rect_b.w && point.x > rect_b.x) &&
		(point.y < rect_b.y + rect_b.h && point.y > rect_b.y);
}

