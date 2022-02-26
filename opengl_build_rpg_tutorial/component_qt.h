#pragma once
#include "component.h"
#include "frarr.h"
#include "rect.h"
#include "component_collider.h"
#include "component_trigger.h"
#include "component_system.h"
#include "component_qtkey.h"

constexpr auto MAX_INDEX = 4;
constexpr auto MAX_LEVEL = 10; // we don't want our quadtree to expand inifinitely due to 11 objects being on the same position somehow

namespace Component
{
	template <typename T>
	class QuadTree
	{
		Rect rect_;
		std::array<QuadTree<T>*, MAX_INDEX> trees_;
		FRArr<IQuadTreeKey*> tree_keys_;
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

		void remove(GLuint id)
		{
			tree_keys_.remove(id);

			if (id <= tree_keys_.size)
			{
				auto& tree_key = tree_keys_[id];
				for (auto i = 0; i < tree_key->locations.size(); i++)
					if (qt == tree_key->locations[i].tree)
						tree_key->locations[i].id = id;
			}
		}

		void split(Entity* master)
		{
			if (level_ + 1 > MAX_LEVEL)
				Logger::error("QuadTree exceeds max level", Logger::SEVERITY::HIGH);

			auto x = rect_.x;
			auto y = rect_.y;
			auto sub_w = rect_ / 2.0f;
			auto sub_h = rect_ / 2.0f;

			// creates 4 new quad tree and adds entities to the prior tree's children
			trees_[top_left] = master->push_back_component<QuadTree>(level_ + 1, tree_keys_.max_size, Rect{ x, y, sub_w, sub_h });
			trees_[top_right] = master->push_back_component<QuadTree>(level_ + 1, tree_keys_.max_size, Rect{ x + sub_w, y, sub_w, sub_h });
			trees_[bot_right] = master->push_back_component<QuadTree>(level_ + 1, tree_keys_.max_size, Rect{ x + sub_w, y + sub_h, sub_w, sub_h });
			trees_[bot_left] = master->push_back_component<QuadTree>(level_ + 1, tree_keys_.max_size, Rect{ x, y + sub_h, sub_w, sub_h });
		}

		void retrieve(Rect rect, std::vector<T*>& retrieved_qt_keys)
		{
			if (trees_[top_left])
				for (auto in = index(rect);;)
				{
					auto in_tree = qt->trees[in % MAX_INDEX];
					retrieve(rect, retrieved_qt_keys);
					in >>= 2;
					if (!in) return;
				}

			for (auto i = 0; i < tree_keys_.size; i++)
				retrieved_qt_keys.push_back(tree_keys_[i]->output_);
		}

		void insert(IQuadTreeKey* key, Entity* master)
		{
			if (tree_keys_.is_full()) {
				split(Entity * master);
				for (auto i = 0u; i < tree_keys_.size; i++)
				{
					auto key_i = tree_keys_[i];

					for (auto in = index(key_i->transform_);;)
					{
						auto in_tree = trees_[in % MAX_INDEX];
						in_tree->insert(key_i, master);
						in >>= 2;
						if (!in) break;
					}
				}
				tree_keys_.clear();
			}

			if (trees_[top_left])
				for (auto in = index(key);;) {
					auto in_tree = trees_[in % MAX_INDEX];
					in_tree->insert(key, master);
					in >>= 2;
					if (!in) return;
				}

			tree_keys_.push_back(key);
		}

		QuadTree(GLuint level, GLuint max_objects, Rect rect)
			: level(level), tree_keys(max_objects), transform(transform), trees{}
		{}
	public:
		friend class QuadTreeKey;

		QuadTree(GLuint max_objects, Rect rect)
			: QuadTree(0, max_objects, rect)
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

			for (auto i = 0; i < tree_keys_.size; i++)
			{
				auto& transform = tree_keys_[i]->transform_;
				os << std::string(level_, '\t') << "(" << transform.x << ", " << transform.y << ", "
					<< transform.w << ", " << transform.h << ")" << std::endl;
			}
		}

		std::vector<T*> retrieve(Rect rect)
		{
			std::vector<T*> retrieved;
			retrieve(rect, retrieved);
			return retrieved;
		}

		static void insert(T& output, Component::Transform& transform, Entity* master)
		{
			auto& c_quadtree = *master->get_component<Component::QuadTree>(0);
			auto key = master->push_back_component<Component::IQuadTreeKey>(output, transform);
			c_quadtree.insert(key, master);
		}
	};
#define GJKQuadTree QuadTree<Component::Collider::IGJK>
#define AABBQuadTree QuadTree<Component::Collider::AABB>
#define SystemQuadTree QuadTree<Component::ISystem>
#define TriggerQuadTree QuadTree<Component::ITrigger>
}