#pragma once
#include "component_qt.h"
#include "component_transform.h"
#include "entity.h"

/*

*/

namespace Component {
	namespace QuadTreeKey 
	{
		template <typename T>
		struct Location : public IQuadTreeKey<T>
		{
			struct QuadTreeLocation
			{
				QuadTree<T>* tree;
				GLuint id;

				QuadTreeLocation(QuadTree<T>* tree, GLuint id)
					: tree(tree), id(id)
				{}
			};
			bool is_retrieved_;
			std::vector<QuadTreeLocation> locations_;
		public:

			std::vector<T*> retrieve()
			{
				std::vector<T*> retrieved_keys;

				is_retrieved_ = true;
				//iterate through key locations and grab all other objects
				for (auto i = 0; i < locations_.size(); i++)
				{
					auto temp_keys = locations_[i].tree->tree_keys_;
					for (auto j = 0u; j < temp_keys.size; j++)
						if (!temp_keys[j]->is_retrieved_)
						{
							retrieved_keys.push_back(temp_keys[j]->output_);
							temp_keys[j]->is_retrieved_ = true;
						}
				}

				for (auto& rk : retrieved_keys)
					rk->is_retrieved_ = false;

				is_retrieved_ = false;
				return retrieved_keys;
			}

			void remove()
			{
				for (auto l : locations_)
				{
					auto tree = l.tree;
					auto id = l.id;
					tree->remove(id);
				}
			}

			void refresh(Entity* master_tree)
			{
				for (auto i = 0; i < locations_.size(); ++i)
				{
					auto tree = locations_[i].tree;
					auto id = locations_[i].id;

					tree->remove(id);
				}
				locations_.clear();

				insert(master_tree);
			}

			void insert(Entity* master_tree)
			{
				auto& c_quad_tree = *master_tree->get_component<Component::QuadTree>(0);
				auto& tree_keys = c_quad_tree.tree_keys_;
				if (tree_keys.is_full()) {
					c_quad_tree.split(Entity * master);
					for (auto i = 0u; i < tree_keys.size; i++)
					{
						auto key_i = tree_keys[i];

						key_i->locations_.clear();

						for (auto in = index(key_i->transform_);;)
						{
							auto in_tree = c_quad_tree.trees_[in % MAX_INDEX];
							in_tree->insert(key_i, master);
							in >>= 2;
							if (!in) break;
						}
					}
					tree_keys.clear();
				}

				if (c_quad_tree.trees_[top_left])
					for (auto in = index(key);;) {
						auto in_tree = c_quad_tree.trees_[in % MAX_INDEX];
						in_tree->insert(key, master);
						in >>= 2;
						if (!in) return;
					}

				tree_keys.push_back(key);
				key->locations_.push_back(QuadTreeKey::QuadTreeLocation(this, tree_keys.size - 1));
			}
		};

#define ColliderQuadTreeKeyRefresh QuadTreeKey::Refresh<Component::ICollider>
	}
}