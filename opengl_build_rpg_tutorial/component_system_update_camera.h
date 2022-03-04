#pragma once
#include "component_system.h"
#include "component_rect.h"
#include "game.h"
#include "component_position.h"

/*
Sets up camera position with respect to the target its follwing, such that it is within the center of the screen, 
however if object is out of boundary camera will not follow

@author David Wadsworth
*/


namespace Component {
	namespace System {
		namespace Update 
		{
			class Camera : public Component::ISystem
			{
				Component::Transform& follower_;
				Component::Position& camera_;
			public:
				Camera(Component::Transform& follower, Component::Position& camera)
					: follower_(follower), camera_(camera)
				{}

				void execute() override
				{
					auto& cam_x = camera_.x;
					auto& cam_y = camera_.y;

					// set camera position so the follower is always at the center
					cam_x = follower_.x - static_cast<float>(Game::width / 2.0f) + follower_.w / 2.0f;
					cam_y = follower_.y - static_cast<float>(Game::height / 2.0f) + follower_.h / 2.0f;

				}
			};
		}
	}
}
