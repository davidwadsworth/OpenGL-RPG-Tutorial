#pragma once
#include "component_system.h"
#include "component_transform.h"

namespace ComponentSystemUpdate
{
	class Camera : Component::System
	{
	public:
		Camera(Component::Transform& follower, Component::Transform& camera)
		{
			auto& camera_pos = camera_transform_.position;

			camera_pos.x = follow_transform_.position.x - static_cast<float>(Game::width / 2) + follow_transform_.width * follow_transform_.scale / 2;
			camera_pos.y = follow_transform_.position.y - static_cast<float>(Game::height / 2) + follow_transform_.height * follow_transform_.scale / 2;

			if (camera_pos.x < 0)
				camera_pos.x = 0;
			if (camera_pos.y < 0)
				camera_pos.y = 0;
			if (camera_pos.x > camera_transform_.width * camera_transform_.scale - Game::width)
				camera_pos.x = camera_transform_.width * camera_transform_.scale - Game::width;
			if (camera_pos.y > camera_transform_.height * camera_transform_.scale - Game::height)
				camera_pos.y = camera_transform_.height * camera_transform_.scale - Game::height;
		}
	};
}
