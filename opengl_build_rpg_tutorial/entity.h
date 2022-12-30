#pragma once
#include <vector>
#include "component.h"
#include <memory>
#include <array>
#include "glad/glad.h"
#include <iostream>
#include "splay_tree.h"
#include "logger.h"
#include <string>

using component_id = std::size_t;

inline component_id get_new_component_id()
{
	static component_id last_id = 0u;
	return ++last_id;
}

// depending on how many components there are creates a seperate get_component_type_if for each, thus giving the illusion of unique id.
template <typename T> inline component_id get_component_id() noexcept
{
	static component_id type_id = get_new_component_id();
	return type_id;
}

/*
storage class for components and other entities beneath it
can either id and retrieve components/children, or treat them as lists.

@author David Wadsworth
*/

class Entity
{
private:
	SplayTree<IComponent> components_;
	SplayTree<Entity> children_;
public:	
	static long long count; // count entity references
	
	Entity()
		: components_(), children_()
	{
		++count;
	}

	~Entity()
	{
		--count;
	}

	// delete all functions that could possibly copy one entity onto another
	Entity(const Entity&) = delete;
	Entity(Entity&&) = delete;
	Entity& operator=(const Entity&) = delete;
	Entity& operator=(Entity&&) = delete;

	Entity* get_child(std::size_t pos)
	{
		auto r_child = children_.search(pos);

		if (!r_child)
			Logger::error("could not find child, pos = " + std::to_string(pos), Logger::HIGH);

		return r_child;
	}

	Entity* get_child(std::string id)
	{
		auto hashed_str = std::hash<std::string>{}(id);
		auto r_child = children_.search(hashed_str);

		if (!r_child)
			Logger::error("could not find child, str id = " + id, Logger::HIGH);

		return r_child;
	}

	std::vector<Entity*> get_child_list()
	{
		return children_.get_ordered_list();
	}

	bool has_child(std::string id)
	{
		auto hashed_str = std::hash<std::string>{}(id);
		return children_.search(hashed_str);
	}

	void remove_child(std::string id)
	{
		auto hashed_str = std::hash<std::string>{}(id);
		children_.remove(hashed_str);
	}

	std::size_t children_size()
	{
		return children_.size();
	}

	// inserts entity into child tree, and treats it like an array
	Entity* push_back_child(Entity* entity) 
	{
		return children_.insert(entity);
	}

	// inserts entity into child tree with hash of str as unique id
	Entity* add_id_child(Entity* entity, std::string id)
	{
		auto hashed_str = std::hash<std::string>{}(id);
		return children_.insert(hashed_str, entity);
	}

	Entity* add_id_child(std::string id)
	{
		auto e = new Entity();
		auto hashed_str = std::hash<std::string>{}(id);
		return children_.insert(hashed_str, e);
	}

	// inserts entity into child tree with unique id
	Entity* add_id_child(Entity* entity, std::size_t id)
	{
		return children_.insert(id, entity);
	}

	// gets component of matching unique id, throws if not there
	template <typename T>
	T* get_component()
	{
		auto r_comp = components_.search(get_component_id<T>());

		if (!r_comp)
			Logger::error("could not find component, component id = " + std::to_string(get_component_id<T>()), Logger::HIGH);

		auto casted_r_comp = dynamic_cast<T*>(r_comp);

		if (!casted_r_comp)
			Logger::error("component not of casted type", Logger::HIGH);

		return casted_r_comp;
	}

	// searches component tree for templated component at pos
	template <typename T>
	T* get_component(std::size_t pos)
	{
		auto r_comp = components_.search(pos);

		if (!r_comp)
			Logger::error("could not find component, pos = " + std::to_string(pos), Logger::HIGH);

		auto casted_r_comp = dynamic_cast<T*>(r_comp);

		if (!casted_r_comp)
			Logger::error("component not of casted type, pos = " + std::to_string(pos), Logger::HIGH);

		return casted_r_comp;
	}

	// searches component tree for specific component of hashed string
	template <typename T>
	T* get_component(std::string id)
	{
		auto hashed_str = std::hash<std::string>{}(id);
		auto r_comp = components_.search(hashed_str);

		if (!r_comp)
			Logger::error("could not find component, str id = " + id, Logger::HIGH);

		auto casted_r_comp = dynamic_cast<T*>(r_comp);

		if (!casted_r_comp)
			Logger::error("component not of casted type, str id = " + id, Logger::HIGH);


		return casted_r_comp;
	}

	// checks if unique id of component is located in component tree
	template <typename T> bool has_component()
	{
		return components_.search(get_component_id<T>());
	}

	// checks if string id of component is located in component tree
	template <typename T> bool has_component(std::string id)
	{
		auto hashed_str = std::hash<std::string>{}(id);
		return components_.search(hashed_str);
	}

	// checks if id of component is located in component tree
	template <typename T> bool has_component(std::size_t pos)
	{
		return components_.search(pos);
	}

	// adds component to splay tree and gives it a unique id, can't have two of the same component per entity
	template<typename T, typename... TArgs>
	T* add_component(TArgs&&... args)
	{
		static_assert (std::is_base_of<IComponent, T>::value, "add_component() T not a component");
		T* c(new T(std::forward<TArgs>(args)...));
		components_.insert(get_component_id<T>(), c);
		return c;
	}

	// adds component to splay tree using a hashed string
	template<typename T, typename... TArgs>
	T* add_id_component(std::string id, TArgs&&... args)
	{
		static_assert (std::is_base_of<IComponent, T>::value, "add_id_component(std::string) T not a component");
		T* c(new T(std::forward<TArgs>(args)...));
		auto hashed_str = std::hash<std::string>{}(id);
		components_.insert(hashed_str, c);
		return c;
	}

	// adds component to splay tree using a hashed string
	template<typename T, typename... TArgs>
	T* add_id_component(std::size_t id, TArgs&&... args)
	{
		static_assert (std::is_base_of<IComponent, T>::value, "add_id_component(std::size_t) T not a component");
		T* c(new T(std::forward<TArgs>(args)...));
		components_.insert(id, c);
		return c;
	}

	// adds component to splay tree and treats it like an array, can have two of the same component in an entity
	template<typename T, typename... TArgs>
	T* push_back_component(TArgs&&... args)
	{
		static_assert (std::is_base_of<IComponent, T>::value, "push_back_component() not a component");
		T* c(new T(std::forward<TArgs>(args)...));
		components_.insert(c);
		return c;
	}

	// gets array of components
	template<typename T>
	std::vector<T*> get_component_list()
	{
		std::vector<T*> tcomp_list;
		for (auto comp : components_.get_ordered_list())
			tcomp_list.push_back(dynamic_cast<T*>(comp));
		return tcomp_list;
	}	

	// delete all components and children
	void clear()
	{
		children_.clear();
		components_.clear();
	}

	// delete all entity children
	void clear_children()
	{
		children_.clear();
	}
};
