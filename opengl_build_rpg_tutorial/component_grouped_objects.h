#pragma once
#include <vector>
#include "component.h"
#include "component_system.h"
#include "frarr.h"
#include "logger.h"
#include "sort.h"


// this probably should be defined out of code but its here for now
#define MAX_CONTAINER_SIZE 128ull

namespace Component
{
	class GroupedSystems
	{
	public:

		struct Item
		{
			Item(std::vector<Component::ISystem*> sys, std::string iid)
				: items(sys), iid(iid)
			{}

			Item(Component::ISystem* sys, std::string iid)
				: iid(iid)
			{
				items.push_back(sys);
			}

			std::vector<Component::ISystem*> items;
			std::string iid;

			bool operator==(const Item& item) { return this->iid == item.iid; }
			bool operator==(Item&& item) { return this->iid == item.iid; }
			bool operator>(const Item& item) { return this->iid > item.iid; }
			bool operator>(Item&& item) { return this->iid < item.iid; }
			bool operator<(const Item& item) { return this->iid > item.iid; }
			bool operator<(Item&& item) { return this->iid < item.iid; }
			bool operator<=(const Item& item) { return *this == item || *this < item; }
			bool operator<=(Item&& item) { return *this == item || *this < item; }
			bool operator>=(const Item& item) { return *this == item || *this > item; }
			bool operator>=(Item&& item) { return *this == item || *this > item; }
		};

		struct Group : public FRArr<Item*>
		{
			Group(float gid)
				: FRArr<Item>(MAX_CONTAINER_SIZE), gid(gid)
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
		
		GroupedSystems()
		{}

		void add(Component::ISystem* obj, std::string iid, float gid)
		{
			auto item = new Item(obj, iid);

			auto j = -1;
			for (auto i = 0; i < groups.size(); ++i, ++j)
			{
				if (groups[i].gid == gid)
				{
					groups[i].push_back(item);
					return;
				}
					
				if (j >= 0)
				{
					if (groups[i].gid > gid)
					{
						Group group(gid);
						group.push_back(item);
						groups.insert(groups.begin() + j, group);
						return;
					}
				}
			}
			
			Group group(gid);
			group.push_back(item);
			groups.push_back(group);
		}

		void add(Component::ISystem* sys, std::string name)
		{
			add(sys, name, static_cast<float>(groups.size()));
		}

		void add(std::vector<Component::ISystem*> sys, std::string iid, float gid)
		{
			auto item = new Item(sys, iid);
			items.push_back(std::unique_ptr<Item>(std::move(item)));

			auto j = -1;
			for (auto i = 0; i < groups.size(); ++i, ++j)
			{
				if (groups[i].gid == gid)
				{
					groups[i].push_back(item);
					return;
				}

				if (j >= 0)
				{
					if (groups[i].gid > gid)
					{
						Group group(gid);
						group.push_back(item);

						groups.insert(groups.begin() + j, group);
						return;
					}
				}
			}

			Group group(gid);
			group.push_back(item);
			groups.push_back(group);
		}
	
		Group* search(const Group& group)
		{
			
		}

		void add(std::vector<Component::ISystem*> objs, std::string iid)
		{
			add(objs, iid, static_cast<float>(groups.size()));
		}

		void add(Group& group)
		{

		}

		void clean(std::string iid, float gid)
		{
			for (auto group : groups)
			{
				if (group.gid == gid)
				{
					for (auto i = 0ull; i < group.size; ++i)
					{
						if (group[i]->iid == iid)
							group.remove(i--);
					}
				}
			}
		}

		void clear()
		{
			for (auto obj_frarr : groups)
				obj_frarr.clear();

			items.clear();
		}

		std::vector<Group> groups;
		std::vector<std::unique_ptr<Item>> items;
	
		private:
			Group* search(std::vector<Group> groups, int m, int l, int r)
			{
				if (l > r)
					return nullptr;

				if (groups[middle] == group)
					return &groups[middle];

				if (groups[middle] > group)
				{
					Group[]
				}
			}
	};
}