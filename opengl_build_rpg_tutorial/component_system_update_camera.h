#pragma once
#include "component_system.h"
#include "component_transform.h"
#include "game.h"

namespace ComponentSystemUpdate
{
	class Camera : public Component::System
	{
		Component::Transform& follower_, &camera_;
	public:
		Camera(Component::Transform& follower, Component::Transform& camera)
			: follower_(follower), camera_(camera)
		{}

		void execute() override
		{
			auto& cam_x = camera_.rect.x;
			auto& cam_y = camera_.rect.y;

			// set camera position so the follower is always at the center
			cam_x = follower_.rect.x - static_cast<float>(Game::width / 2) + follower_.rect.w * follower_.scale / 2;
			cam_y = follower_.rect.y - static_cast<float>(Game::height / 2) + follower_.rect.h * follower_.scale / 2;

			// if reaches one of the lower or upper bounds make it so the camera doesn't follow past
			if (cam_x < 0)
				cam_x = 0;
			if (cam_y < 0)
				cam_y = 0;
			if (cam_x > camera_.rect.w * camera_.scale - Game::width)
				cam_x = camera_.rect.w * camera_.scale - Game::width;
			if (cam_y > camera_.rect.w * camera_.scale - Game::height)
				cam_y = camera_.rect.h * camera_.scale - Game::height;
		}
	};
}
