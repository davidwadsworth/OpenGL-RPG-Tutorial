#pragma once
#include "component.h"
#include <vector>
#include "component_rect.h"
#include "logger.h"
#include "game.h"

namespace Component
{
	class Cursor : public IComponent
	{
		std::vector<Component::Transform*> cursor_transforms_;
		float cursor_width_, cursor_height_;
		float cursor_pos_;
	public:
		Cursor(float cursor_width, float cursor_height)
			: cursor_width_(cursor_width), cursor_height_(cursor_height), cursor_pos_(0.0f)
		{}

		void increment()
		{
			if (!cursor_transforms_.size())
				Logger::error("cursor transforms not set up", Logger::HIGH);

			auto current_cursor = cursor_transforms_[static_cast<int>(cursor_pos_)];

			current_cursor->set(current_cursor->x, current_cursor->y, 0.0f, 0.0f);

			cursor_pos_ += Game::delta_time;

			if (cursor_pos_ > cursor_transforms_.size() - 1)
				cursor_pos_ = 0;
			
			cursor_transforms_[static_cast<int>(cursor_pos_)]->set(current_cursor->x, current_cursor->y, cursor_width_, cursor_height_);
		}

		int get_cursor_size() { return static_cast<int>(cursor_transforms_.size()); }

		void decrement()
		{
			if (!cursor_transforms_.size())
				Logger::error("cursor transforms not set up", Logger::HIGH);

			auto current_cursor = cursor_transforms_[static_cast<int>(cursor_pos_)];

			current_cursor->set(current_cursor->x, current_cursor->y, 0.0f, 0.0f);

			cursor_pos_ -= Game::delta_time;

			if (cursor_pos_ < 0)
				cursor_pos_ = cursor_transforms_.size() - 1;

			cursor_transforms_[static_cast<int>(cursor_pos_)]->set(current_cursor->x, current_cursor->y, cursor_width_, cursor_height_);
		}

		int get_cursor_pos() { return cursor_pos_; }

		void add_options(std::vector<Component::Transform*> transforms)
		{
			cursor_transforms_ = transforms;
		}
	};
}