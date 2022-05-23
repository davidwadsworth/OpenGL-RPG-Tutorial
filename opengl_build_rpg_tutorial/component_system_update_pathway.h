#pragma once
#include "component_navigator.h"
#include "component_system.h"
#include "component_trigger.h"
#include "tree.h"

namespace Component {
	namespace System {
		namespace Update
		{
			class Pathway : public Component::ISystem
			{
				Component::Navigator& nav_;
				std::vector<std::unique_ptr<Tree<std::vector<Component::ITrigger*>>>> created_trees_;
				Tree<std::vector<Component::ITrigger*>>* curr_tree_;
			public:
				Pathway(Component::Navigator& nav)
					: curr_tree_(nullptr), nav_(nav)
				{}


				Tree<std::vector<Component::ITrigger*>>* add_path(std::vector<ITrigger*> triggers)
				{
					auto n_tree = new Tree<std::vector<Component::ITrigger*>>{ {},  triggers };
					created_trees_.push_back(std::unique_ptr<Tree<std::vector<Component::ITrigger*>>>(std::move(n_tree)));

					curr_tree_ = n_tree;
					return n_tree;
				}

				void clear_trees()
				{
					created_trees_.clear();
				}

				void set_curr(Tree<std::vector<Component::ITrigger*>>* tree)
				{
					curr_tree_ = tree;
				}

				void execute() override
				{

				}
			};
		}
	}
}