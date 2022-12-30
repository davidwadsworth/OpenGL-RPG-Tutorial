#pragma once
#include "add_navigator.h"
#include "component_system.h"
#include <unordered_map>
#include <stack>
#include <queue>
#include "component_vector.h"
#include "component_command.h"
#include "add_loads.h"
#include "ring_buffer.h"

namespace Component {
	namespace Command
	{
		class Pathway : public Component::Command
		{
		public:
			struct NavigatorTree
			{
				nlohmann::json command_json;
				std::vector<NavigatorTree*> children;
				void clear()
				{
					command_json.clear();
					children.clear();
					navigator = nullptr;
				}
				INavigator* navigator = nullptr;
			};

			struct NavigatorTreeItem
			{
				NavigatorTreeItem(NavigatorTree* tree, std::size_t size = 1u)
					:tree(tree), size(size)
				{}

				NavigatorTree* tree;
				std::size_t size;
			};
		private:
			RingBuffer<NavigatorTree> navigator_trees_;
			std::unordered_map<std::string, std::unique_ptr<INavigator>> nav_map_;
			std::unordered_map<std::string, ICommand*> command_map_;
			std::queue<NavigatorTreeItem> nav_queue_;
		public:
			Pathway()
			{
				add_navigator(nav_map_);
				add_loads(command_map_);
			}

			void add_nav(std::string name, nlohmann::json json, Entity* gamestate )
			{
				if (nav_map_.find(name) == nav_map_.end())
				{
					auto nav = add_navigator(json, name, gamestate);
					nav_map_[name] = std::make_unique<INavigator>(nav);
				}
			}

			NavigatorTree* get_nav_tree()
			{
				return navigator_trees_.get();
			}


			void change_nav(std::string nav, NavigatorTree* tree)
			{
				tree->navigator = nav_map_[nav].get();
			}

			void add_command(std::string name, ICommand* command)
			{
				if (command_map_.find(name) != command_map_.end())
					command_map_[name] = command;
			}

			void add_next(NavigatorTree* tree, std::size_t size)
			{
				nav_queue_.push(NavigatorTreeItem(tree, size));
			}

			void add_next(nlohmann::json json)
			{
				auto tree = get_nav_tree();
				tree->command_json = json;
				nav_queue_.push(NavigatorTreeItem(tree));
			}

			void clear()
			{
				while (!nav_queue_.empty()) nav_queue_.pop();
			}

			void execute(Entity * e_gamestate) override
			{
				if (nav_queue_.empty())
				{
					return;
				}
					
				if (nav_tree)
				{
					nav_empty = false;
					auto nav_path = nav_tree->navigator->navigate();

					if (nav_path)
					{
						if (nav_path > 0 && nav_path >= nav_tree->children.size())
						{
							nav_tree = nav_tree->children[nav_path];
							for (auto command_json : nav_tree->command_json)
							{
								auto command = command_map_[command_json["name"].get<std::string>()];

								if (command_json.contains("load"))
								{
									auto load = dynamic_cast<ILoad*>(command);
									load->load(command_json["load"]);
									load->execute(gamestate);
								}

							}

						}
					}
				}
				else
					nav_queue_.pop();
			}
		};
	}
}