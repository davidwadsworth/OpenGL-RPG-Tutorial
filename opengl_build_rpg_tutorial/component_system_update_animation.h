#pragma once
#include "component_system.h"
#include "component_src.h"
#include <vector>
#include <unordered_map>
#include <iostream>
#include "GLFW/glfw3.h"

namespace ComponentSystemUpdate
{
	class Animation : public Component::System
	{
#define Anim std::vector<Component::Src&>

		std::unordered_map<std::string, Anim> anim_map_;
		GLfloat speed_;
		bool is_animated_;
		Anim* current_;
		Component::Src& src_;
	public:
		Animation(GLfloat speed, Component::Src& src)
			: speed_(speed), is_animated_(true), current_(nullptr), anim_map_(), src_(src)
		{}

		void execute() override
		{
			if (is_animated_)
			{
				auto time = static_cast<float>(glfwGetTime());
				auto rand_pos = static_cast<std::size_t>(time * speed_) % current_->size();
				auto& rand_src = current_->at(rand_pos);

				src_.x = rand_src.x;
				src_.y = rand_src.y;
				src_.w = rand_src.w;
				src_.h = rand_src.h;
			}
		}

		void add(std::string id, Anim anim)
		{
			anim_map_.emplace(id, anim);
		}

		void play(std::string id)
		{
			current_ = &anim_map_[id];
			is_animated_ = true;
		}
	};
}


