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
		Rect cursor_dimension_;
		float cursor_pos_;
	public:
		Cursor(Rect cursor_dimension)
			: cursor_dimension_(cursor_dimension), cursor_pos_(0.0f)
		{}

		void increment()
		{
			if (!cursor_transforms_.size())
				Logger::error("cursor transforms not set up", Logger::HIGH);

			cursor_transforms_[cursor_pos_]->set();

			cursor_pos_ += Game::delta_time;

			if (cursor_pos_ > cursor_transforms_.size() - 1)
				cursor_pos_ = 0;
			
			cursor_transforms_[static_cast<int>(cursor_pos_)]->set(cursor_dimension_);
		}

		int get_cursor_size() { return static_cast<int>(cursor_transforms_.size()); }

		void decrement()
		{
			if (!cursor_transforms_.size())
				Logger::error("cursor transforms not set up", Logger::HIGH);

			cursor_transforms_[cursor_pos_]->set();

			cursor_pos_ -= Game::delta_time;

			if (cursor_pos_ < 0)
				cursor_pos_ = cursor_transforms_.size() - 1;

			cursor_transforms_[static_cast<int>(cursor_pos_)]->set(cursor_dimension_);
		}

		int get_cursor_pos() { return cursor_pos_; }

		void add_options(std::vector<Component::Transform*> transforms)
		{
			cursor_transforms_ = transforms;
		}
	};
}