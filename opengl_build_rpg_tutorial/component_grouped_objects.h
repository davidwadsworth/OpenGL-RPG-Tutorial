#pragma once
#include <vector>
#include "component.h"
#include "component_system.h"
#include "frarr.h"
#include "logger.h"

namespace Component
{
	template <typename T>
	class GroupedObjects
	{
		struct Group : public FRArr<T>
		{
			Group(std::size_t id)
				: id(id)
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
				


			objects[pos].push_back(obj);
		}

		void add(std::vector<T> objs, std::size_t pos)
		{

		}

		void clear()
		{
			for (auto obj_frarr : objects)
				obj_frarr.clear();
		}

		std::vector<Group>> groups;
	};
#define GroupedSystems GroupedObjects<Component::ISystem*>
}