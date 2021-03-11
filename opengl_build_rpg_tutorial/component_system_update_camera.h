#pragma once
#include "component_system.h"
#include "component_transform.h"
#include "game.h"

/*
Sets up camera position with respect to the target its follwing, such that it is within the center of the screen, 
however if object is out of boundary camera will not follow

@author David Wadsworth
*/

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
			auto& cam_x = camera_.x;
			auto& cam_y = camera_.y;

			// set camera position so the follower is always at the center
			cam_x = follower_.x - static_cast<float>(Game::width / 2) + follower_.w * follower_.sc / 2;
			cam_y = follower_.y - static_cast<float>(Game::height / 2) + follower_.h * follower_.sc / 2;

			// if reaches one of the lower or upper bounds make it so the camera doesn't follow past
			if (cam_x < 0)
				cam_x = 0;
			if (cam_y < 0)
				cam_y = 0;
			if (cam_x > camera_.w * camera_.sc - Game::width)
				cam_x = camera_.w * camera_.sc - Game::width;
			if (cam_y > camera_.w * camera_.sc - Game::height)
				cam_y = camera_.h * camera_.sc - Game::height;
		}
	};
}
