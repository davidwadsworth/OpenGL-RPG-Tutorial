#pragma once
#include "component.h"
#include "frarr.h"
#include "rect.h"
#include "component_trigger.h"
#include "component_system.h"
#include "component_rect_gjk_physics.h"

constexpr auto MAX_INDEX = 4;
constexpr auto MAX_LEVEL = 10; // we don't want our quadtree to expand inifinitely due to 11 objects being on the same position somehow

/*
TODO

@author David Wadsworth
*/

namespace Component
{
	template <typename T>
	class QuadTree : public IComponent
	{
		Rect rect_;
		std::array<QuadTree*, MAX_INDEX> trees_;
		FRArr<T*> objects_;
		GLuint level_;

		enum quadrants
		{
			top_left,
			top_right,
			bot_right,
			bot_left
		};

		GLuint index(Rect rect)
		{
			auto vertical_midpoint = rect_.x + (rect_.w / 2.0f);
			auto horizontal_midpoint = rect_.y + (rect_.h / 2.0f);

			auto x = rect.x;
			auto y = rect.y;
			auto w = rect.w;
			auto h = rect.h;

			auto top_quadrant = y < horizontal_midpoint;
			auto bottom_quadrant = y + h > horizontal_midpoint;

			auto left_quadrant = x < vertical_midpoint;
			auto right_quadrant = x + w > vertical_midpoint;


			auto i = 0u;
			auto index = 0u;

			if (left_quadrant) {
				if (top_quadrant) index += top_left << (i++ << 1u);
				if (bottom_quadrant) index += bot_left << (i++ << 1u);
			}
			if (right_quadrant) {
				if (top_quadrant) index += top_right << (i++ << 1u);
				if (bottom_quadrant) index += bot_right << (i++ << 1u);
			}
			return index;
		}

		void split(Entity* master)
		{
			if (level_ + 1 > MAX_LEVEL)
				Logger::error("QuadTree exceeds max level", Logger::SEVERITY::HIGH);

			auto x = rect_.x;
			auto y = rect_.y;
			auto sub_w = rect_.w / 2.0f;
			auto sub_h = rect_.h / 2.0f;

			// creates 4 new quad tree and adds entities to the prior tree's children
			trees_[top_left] = master->push_back_component<QuadTree>(level_ + 1, objects_.max_size, Rect( x, y, sub_w, sub_h ));
			trees_[top_right] = master->push_back_component<QuadTree>(level_ + 1, objects_.max_size, Rect( x + sub_w, y, sub_w, sub_h ));
			trees_[bot_right] = master->push_back_component<QuadTree>(level_ + 1, objects_.max_size, Rect( x + sub_w, y + sub_h, sub_w, sub_h ));
			trees_[bot_left] = master->push_back_component<QuadTree>(level_ + 1, objects_.max_size, Rect( x, y + sub_h, sub_w, sub_h ));
		}

		void retrieve(Rect rect, std::vector<T*>& retrieved_objs)
		{
			if (trees_[top_left])
				for (auto in = index(rect);;)
				{
					auto in_tree = trees_[in % MAX_INDEX];
					retrieve(rect, retrieved_objs);
					in >>= 2;
					if (!in) return;
				}

			for (auto i = 0u; i < objects_.size; i++)
				retrieved_objs.push_back(objects_[i]);
		}

		void insert(T* obj, Entity* master)
		{
			if (objects_.is_full()) {
				split(master);
				for (auto i = 0u; i < objects_.size; i++)
				{
					auto key_i = objects_[i];

					for (auto in = index(*key_i);;)
					{
						auto in_tree = trees_[in % MAX_INDEX];
						in_tree->insert(key_i, master);
						in >>= 2;
						if (!in) break;
					}
				}
				objects_.clear();
			}

			if (trees_[top_left])
				for (auto in = index(*obj);;) {
					auto in_tree = trees_[in % MAX_INDEX];
					in_tree->insert(obj, master);
					in >>= 2;
					if (!in) return;
				}

			objects_.push_back(obj);
		}

	public:

		QuadTree(GLuint max_objects, Rect rect)
			: QuadTree(0, max_objects, rect)
		{}

		QuadTree(GLuint level, GLuint max_objects, Rect rect)
			: level_(level), objects_(max_objects), rect_(rect), trees_{}
		{}

		void print(std::ostream& os)
		{
			os << std::endl;

			if (trees_[top_left])
			{
				auto t_tl = trees_[top_left]->rect_;
				os << std::string(level_, '\t') << "(" << t_tl.x << ", " << t_tl.y << ", "
					<< t_tl.w << ", " << t_tl.h << ")";
				trees_[top_left]->print(os);

				auto t_tr = trees_[top_right]->rect_;
				os << std::string(level_, '\t') << "(" << t_tr.x << ", " << t_tr.y << ", "
					<< t_tr.w << ", " << t_tr.h << ")";
				trees_[top_right]->print(os);

				auto t_br = trees_[bot_right]->rect_;
				os << std::string(level_, '\t') << "(" << t_br.x << ", " << t_br.y << ", "
					<< t_br.w << ", " << t_br.h << ")";
				trees_[bot_right]->print(os);

				auto t_bl = trees_[bot_left]->rect_;
				os << std::string(level_, '\t') << "(" << t_bl.x << ", " << t_bl.y << ", "
					<< t_bl.w << ", " << t_bl.h << ")";
				trees_[bot_left]->print(os);

			}

			for (auto i = 0u; i < objects_.size; i++)
			{
				auto& transform = *objects_[i];
				os << std::string(level_, '\t') << "(" << transform.x << ", " << transform.y << ", "
					<< transform.w << ", " << transform.h << ")" << std::endl;
			}
		}

		static void add(T* obj, Entity* master)
		{
			auto& c_quad_tree = *master->get_component<Component::QuadTree<T>>(0);
			c_quad_tree.insert(obj, master);
		}

		std::vector<T*> retrieve(Rect rect)
		{
			std::vector<T*> retrieved;
			retrieve(rect, retrieved);
			return retrieved;
		}
	};
#define PhysicsGJKQTree QuadTree<Component::Rectangle::GJK::IPhysics<Component::Rectangle::IGJK>>
#define PhysicsActionGJKQTree QuadTree<Component::Rectangle::GJK::IPhysics<Component::Rectangle::GJK::Action>>
#define ActionQTree QuadTree<Component::Rectangle::Action>
}
