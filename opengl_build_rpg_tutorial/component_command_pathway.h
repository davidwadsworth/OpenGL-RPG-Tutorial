#pragma once
#include "add_navigator.h"
#include "component_system.h"
#include <unordered_map>
#include <stack>
#include <queue>
#include "component_vector.h"
#include "add_loads.h"
#include "ring_buffer.h"
#include "component_command.h"
#include "command_load.h"

namespace Component {
	namespace Command
	{
		class Pathway : public Component::CCommand
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
			bool removed_;
			NavigatorTree* curr_;
			RingBuffer<NavigatorTree> navigator_trees_;
			std::unordered_map<std::string, std::unique_ptr<INavigator>> nav_map_;
			std::unordered_map<std::string, std::unique_ptr<ICommand>> command_map_;
			std::queue<NavigatorTreeItem> nav_queue_;
		public:
			Pathway(std::size_t max_nav_tree)
				: navigator_trees_(max_nav_tree), curr_(nullptr), removed_(false)
			{}

			void add_nav(std::string name, nlohmann::json json, Entity* gamestate)
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

			void execute(Entity* e_gamestate) override
			{
				if (nav_queue_.empty())
					return;

				if (curr_)
				{
					auto nav_path = curr_->navigator->navigate();
					if (nav_path)
					{
						for (auto command_json : curr_->command_json)
						{
							auto command = command_map_[command_json["name"].get<std::string>()].get();
							command->execute(e_gamestate);
						}
						if (nav_path > 0)
						{
							if (curr_->children.size() == 0)
							{
								curr_ = nullptr;
								removed_ = true;
							}
							else if (nav_path <= curr_->children.size())
							{
								curr_ = curr_->children[nav_path - 1];
								curr_->navigator->init(e_gamestate);
								for (auto command_json : curr_->command_json)
								{
									auto command = command_map_[command_json["name"].get<std::string>()].get();
									auto load = dynamic_cast<Command::ILoad*>(command);
									if (load)
										load->load(command_json["load"]);
								}
							}
						}
					}
				}
				else
				{
					if (removed_)
					{
						curr_ = nullptr;
						navigator_trees_.remove(nav_queue_.front().size);
						nav_queue_.pop();
						removed_ = false;
						return;
					}
					curr_ = nav_queue_.front().tree;
				}
			}
		};

	}
}