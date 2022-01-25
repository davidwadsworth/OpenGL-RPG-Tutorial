#pragma once
#include <vector>
#include "component.h"
#include "component_system.h"
#include "frarr.h"
#include "logger.h"
#include "sort.h"
#include "component_system_item.h"

namespace Component
{
	class Engine
	{
		struct Group : public std::vector<Component::System::IItem*>
		{
			Group(float gid)
				: gid(gid)
			{}
			float gid;

			// overloading compare operators
			bool operator==(const Group& group) { return this->gid == group.gid; }
			bool operator==(Group&& group) { return this->gid == group.gid; }
			bool operator>(const Group& group) { return this->gid > group.gid; }
			bool operator>(Group&& group) { return this->gid < group.gid; }
			bool operator<(const Group& group) { return this->gid > group.gid; }
			bool operator<(Group&& group) { return this->gid < group.gid; }
			bool operator<=(const Group& group) { return *this == group || *this < group; }
			bool operator<=(Group&& group) { return *this == group || *this < group; }
			bool operator>=(const Group& group) { return *this == group || *this > group; }
			bool operator>=(Group&& group) { return *this == group || *this > group; }
		};

		std::vector<Group> groups_;
		int search(float gid)
		{
			int l = 0;
			int r = groups_.size() - 1;

			while (r > l)
			{
				int m = (r - l) >> 1;

				if (groups_[m].gid == gid)
					return m;

				if (groups_[m].gid > gid)
					l = m;

				if (groups_[m].gid < gid)
					r = m;
			}
			return -1;
		}

	public:

		Engine()
		{}

		void add(Component::System::IItem* item, float gid)
		{	
			int gsr = search(gid);

			if (gsr > 0)
				groups_[gsr].push_back(item);
			else
			{
				Group g(gid);
				g.push_back(item);
				groups_.push_back(g);
				insertion_sort<Group>(groups_);
			}
		}

		void add(Engine& engine)
		{
			for (auto &gr : engine.groups_)
			{
				int gsr = search(gr.gid);

				if (gsr)
					groups_[gsr].insert(groups_[gsr].end(), gr.begin(), gr.end());
				else
				{
					groups_.push_back(gr);
					insertion_sort<Group>(groups_);
				}
			}
		}

		void swap(Engine& engine)
		{
			std::vector<Group> temp_groups;

			temp_groups.insert(temp_groups.begin(), groups_.begin(), groups_.end());
			
			groups_.clear();
			groups_.insert(groups_.begin(), engine.groups_.begin(), engine.groups_.end());

			engine.groups_.clear();
			engine.groups_.insert(engine.groups_.begin(), temp_groups.begin(), temp_groups.end());
		}
		
		void run()
		{
			for (auto i = 0ull; i < groups_.size(); ++i)
				for (auto j = 0ull; j < groups_[i].size(); ++j)
					groups_[i][j]->execute();
		}

		void clear()
		{
			for (auto i = 0; i < groups_.size(); ++i)
				groups_[i].clear();

			groups_.clear();
		}
	};
}