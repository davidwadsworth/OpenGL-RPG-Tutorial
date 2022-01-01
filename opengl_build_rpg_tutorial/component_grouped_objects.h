#pragma once
#include <vector>
#include "component.h"
#include "component_system.h"
#include "frarr.h"
#include "logger.h"


// this probably should be defined out of code but its here for now
#define MAX_CONTAINER_SIZE 128ull

namespace Component
{
	template <typename T>
	class GroupedObjects
	{
		struct Group : public FRArr<T>
		{
			Group(std::size_t id)
				: FRArr<T>(MAX_CONTAINER_SIZE), id(id)
			{}
			std::size_t id;
		};

	public:
		GroupedObjects()
		{}

		void add(T obj, std::size_t pos)
		{
			auto j = -1;
			for (auto i = 0; i < groups.size(); ++i, ++j)
			{
				if (groups[i].id == pos)
				{
					groups[i].push_back(obj);
					return;
				}
					
				if (j >= 0)
				{
					if (groups[i].id > pos)
					{
						groups.insert(groups.begin() + j, Group(id))->push_back(obj);
						return;
					}
				}
			}
			
			objects.push_back(Group(id));
			(objects.end() - 1)->push_back(obj);
		}

		void add(std::vector<T> objs, std::size_t pos)
		{
			auto j = -1;
			for (auto i = 0; i < groups.size(); ++i, ++j)
			{
				if (groups[i].id == pos)
				{
					groups[i].push_back(objs);
					return;
				}

				if (j >= 0)
				{
					if (groups[i].id > pos)
					{
						groups.insert(groups.begin() + j, Group(id))->push_back(objs);
						return;
					}
				}
			}

			objects.push_back(Group(id));
			(objects.end() - 1)->push_back(objs);
		}


		void add(Group& group)
		{
			//TODO
		}

		void remove(Group& group)
		{
			//TODO
		}

		void clear()
		{
			for (auto obj_frarr : objects)
				obj_frarr.clear();
		}

		std::vector<Group> groups;
	};
#define GroupedSystems GroupedObjects<Component::ISystem*>
}