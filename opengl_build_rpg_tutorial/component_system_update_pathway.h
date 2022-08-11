#pragma once
#include "add_navigator.h"
#include "component_system.h"
#include "component_trigger.h"
#include <unordered_map>
#include <stack>
#include <queue>
#include "load.h"
#include "component_vector.h"

namespace Component {
	namespace System {
		namespace Update
		{
			class Pathway : public Component::ISystem
			{
			public:
				struct NavigatorTree
				{
					std::vector<nlohmann::json> command_json;
					std::vector<NavigatorTree*> children;
					INavigator* navigator;
				};
			private:
				std::vector<std::unique_ptr<NavigatorTree>> created_navigators_;
				std::vector<std::unique_ptr<ICommand>> created_commands_;
				std::unordered_map<std::string, std::unique_ptr<INavigator>> nav_map_;
				std::unordered_map<std::string, ICommand*> command_map_;
				std::unordered_map<std::string, NavigatorTree*> nav_tree_map_;
				std::queue<std::vector<NavigatorTree*>> nav_queue_;
				Component::CommandVector& command_vec_;
			public:
				Pathway(Component::CommandVector& command_vec)
					: command_vec_(command_vec)
				{}

				NavigatorTree* add_path(std::string name, nlohmann::json json, Entity* gamestate, std::vector<nlohmann::json> command_jsons)
				{
					INavigator* nav = nullptr;
					if (nav_map_.find(name) != nav_map_.end())
						nav = nav_map_[name].get();
					else
					{
						nav = add_navigator(json, name, gamestate);
						nav_map_[name] = std::make_unique<INavigator>(nav);
					}
					
					auto nav_tree = new NavigatorTree{ command_jsons, std::vector<NavigatorTree*>(), nav };
					
					return nav_tree;
				}

				ILoad* add_load(std::string name)
				{

				}

				void add_command(std::string name, ICommand* command)
				{

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
										}
										command_vec_.push_back(command);
									}

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