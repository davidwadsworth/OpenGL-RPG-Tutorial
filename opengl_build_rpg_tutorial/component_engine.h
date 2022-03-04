#pragma once
#include <vector>
#include "component.h"
#include "component_system.h"
#include "frarr.h"
#include "logger.h"
#include "sort.h"

namespace Component
{
	class Engine : public IComponent
	{
		struct Group : public std::vector<Component::ISystem*>
		{
			Group()
				: gid(-1.0f)
			{}

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
		std::size_t search(float gid)
		{
			auto l = 0ull;
			auto r = groups_.size() - 1;

			while (r > l)
			{
				auto m = (r - l) >> 1;

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

		~Engine()
		{
			clear();
		}

		void add(Component::ISystem* item, float gid)
		{	
			auto gsr = search(gid);

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
				auto gsr = search(gr.gid);

				if (gsr)
					groups_[gsr].insert(groups_[gsr].end(), gr.begin(), gr.end());
				else
				{
					groups_.push_back(gr);
					insertion_sort<Group>(groups_);
				}
			}
		}

		std::vector<Component::ISystem*> flatten()
		{
			std::vector<Component::ISystem*> flattened_engine;

			for (auto& g : groups_)
				flattened_engine.insert(flattened_engine.end(), g.begin(), g.end());

			return flattened_engine;
		}

		void clear()
		{
			for (auto i = 0; i < groups_.size(); ++i)
				groups_[i].clear();

			groups_.clear();
		}
	};
}