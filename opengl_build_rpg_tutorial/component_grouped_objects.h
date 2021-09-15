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
	public:
		GroupedObjects(std::size_t size)
		{
			for (auto i = 0; i < size; ++i)
				objects.push_back(FRArr<T>());
		}

		void add(T obj, std::size_t pos)
		{
			if (pos >= objects.size())
				Logger::error("GroupedObject pos is out of bounds. Add failed", Logger::HIGH);
			objects[pos].push_back(obj);
		}

		std::vector<FRArr<T>> objects;
	};
#define GroupedSystems GroupedObjects<Component::ISystem*>
}