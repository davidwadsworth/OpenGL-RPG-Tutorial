#pragma once
#include <vector>
#include "component.h"
#include "component_system.h"
#include "frarr.h"
#include "logger.h"
#include "sort.h"
#include "component_system_item.h"


// this probably should be defined out of code but its here for now
#define MAX_CONTAINER_SIZE 128ull

namespace Component
{
	class Engine
	{
		friend Component::System::IItem;
		struct Group : public FRArr<Component::System::IItem*>
		{
			Group(float gid)
				: FRArr<Component::System::IItem*>(MAX_CONTAINER_SIZE), gid(gid)
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

		FRArr<Group> groups_;
		int search(float gid)
		{
			int l = 0;
			int r = groups_.size - 1;

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
			}
		}

		void add(Engine& engine)
		{
			for (auto i = 0ull; i < engine.groups_.size; ++i)
			{
				int gsr = search(engine.groups_[i].gid);
				Group* g = nullptr;

				if (gsr > 0)
					g = &groups_[gsr];
				else
				{
					groups_.push_back(Group(engine.groups_[i].gid));
					g = &groups_[groups_.size - 1];
				}

				for (auto j = 0ull; j < engine.groups_[i].size; ++j)
					g->push_back(engine.groups_[i][j]);
			}
		}
		
		void clean()
		{
			for (auto i = 0ull; i < groups_.size; ++i)
			{
				if (groups_[i].size)
					for (auto j = 0ull; j < groups_[i].size; ++j)
					{
						if (groups_[i][j]->removed_)
							groups_[i].remove(j--);
					}
				else
					groups_.remove(i--);
			}
		}

		void run()
		{
			for (auto i = 0ull; i < groups_.size; ++i)
				for (auto j = 0ull; j < groups_[i].size; ++j)
					groups_[i][j]->execute();
		}

		void clear()
		{
			for (auto i = 0; i < groups_.size; ++i)
				groups_[i].clear();

			groups_.clear();
		}
	};
}