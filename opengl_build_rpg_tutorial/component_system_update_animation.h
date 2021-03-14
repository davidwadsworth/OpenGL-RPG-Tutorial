#pragma once
#include "component_system.h"
#include "component_src.h"
#include <vector>
#include <unordered_map>
#include <iostream>
#include "GLFW/glfw3.h"

/*
Update the next frame to be drawn of our sprite sheet to give the illusion of movement

@author David Wadsworth
*/

namespace ComponentSystemUpdate
{
	class Animation : public Component::System
	{
#define Anim std::vector<Component::Src*>

		std::unordered_map<std::string, Anim> anim_map_;
		GLfloat speed_;
		Anim* current_;
		Component::Src& src_;
	public:
		Animation(GLfloat speed, Component::Src& src)
			: speed_(speed), current_(nullptr), anim_map_(), src_(src)
		{}

		void execute() override
		{
			auto time = static_cast<float>(glfwGetTime());
			auto anim_pos = static_cast<std::size_t>(time * speed_) % current_->size();
			auto anim_src = current_->at(anim_pos);

			src_.x = anim_src->x;
			src_.y = anim_src->y;
			src_.w = anim_src->w;
			src_.h = anim_src->h;
		}

		void add(std::string id, Anim anim)
		{
			anim_map_.emplace(id, anim);
			current_ = &anim;
		}

		void play(std::string id)
		{
			current_ = &anim_map_[id];
		}
	};
}


