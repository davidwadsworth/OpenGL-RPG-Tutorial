#pragma once
#include "add_navigator.h"
#include "component_system.h"
#include "component_trigger.h"
#include <unordered_map>
#include <stack>
#include <queue>

namespace Component {
	namespace System {
		namespace Update
		{
			class Pathway : public Component::ISystem
			{
				struct NavigatorTree
				{
					NavigatorTree* parent;
					std::vector<Component::ITrigger*> triggers;
					std::vector<NavigatorTree*> children;
					INavigator* navigator;
				};

				std::vector<std::unique_ptr<NavigatorTree>> created_navigators_;
				std::unordered_map<std::string, std::unique_ptr<INavigator>> nav_map_;
				std::unordered_map<std::string, NavigatorTree*> nav_tree_map_;
				std::queue<std::vector<NavigatorTree*>> nav_queue_;
			public:
				Pathway()
				{}

				NavigatorTree* add_path(nlohmann::json json, Entity* gamestate, std::vector<ITrigger*> triggers)
				{
					auto nav = add_navigator(json, gamestate);
					auto nav_tree = new NavigatorTree{ nullptr, triggers, std::vector<NavigatorTree*>(), nav };
					

					created_navigators_.push_back(std::make_unique<NavigatorTree>(nav_tree));

					return nav_tree;
				}


				void add_concurrent(std::string name)
				{
					if (nav_queue_.empty())
						nav_queue_.push(std::vector<NavigatorTree*>());
					nav_queue_.front().push_back(nav_tree_map_[name]);
				}

				void register_root(std::string name, NavigatorTree* nav_tree)
				{
					nav_tree_map_[name] = nav_tree;
				}

				bool is_registered(std::string name)
				{
					return nav_tree_map_.find(name) != nav_tree_map_.end();
				}

				void add_next(std::string name)
				{
					std::vector<NavigatorTree*> nav_vec;
					nav_vec.push_back(nav_tree_map_[name]);
					nav_queue_.push(nav_vec);
				}

				void clear()
				{
					created_navigators_.clear();
					while (!nav_queue_.empty()) nav_queue_.pop();
					nav_map_.clear();
				}

				void execute() override
				{
					if (nav_queue_.empty())
						return;

					bool nav_empty = true;
					for (auto& nav_tree : nav_queue_.front())
					{
						if (nav_tree)
						{
							nav_empty = false;
							auto nav_path = nav_tree->navigator->navigate();

							if (nav_path)
							{
								if (nav_path > 0)
								{
									if (nav_path >= nav_tree->children.size())
										Logger::error("Navigation out of bounds", Logger::HIGH);

									nav_tree = nav_tree->children[nav_path];
								}
								else
								{
									if (nav_tree->parent)
										nav_tree = nav_tree->parent;
								}
							}

						}
					}

					if (nav_empty)
						nav_queue_.pop();
				}
			};
		}
	}
}