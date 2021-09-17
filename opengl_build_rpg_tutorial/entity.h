#pragma once
#include <vector>
#include "component.h"
#include <memory>
#include <array>
#include "glad/glad.h"
#include <iostream>
#include "splay_tree.h"
#include "logger.h"

using component_id = std::size_t;

inline component_id get_new_component_id()
{
	static component_id last_id = 0u;
	return ++last_id;
}

// depending on how many components there are creates a seperate get_component_type_if for each, thus giving the illusion of unique id
template <typename T> inline component_id get_component_id() noexcept
{
	// assert that it inherits Comp
	if (!std::is_base_of<IComponent, T>::value)
		Logger::error("Class does not inherit Component.", Logger::HIGH);

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
			Logger::error("could not find child, pos = " + pos, Logger::HIGH);

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

	GLuint children_size()
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
			Logger::error("could not find component, component id = " + get_component_id<T>(), Logger::HIGH);

		return static_cast<T*>(r_comp);
	}

	// searches component tree for templated component at pos
	template <typename T>
	T* get_component(std::size_t pos)
	{
		auto r_comp = components_.search(pos);

		if (!r_comp)
			Logger::error("could not find component, pos = " + pos, Logger::HIGH);

		return static_cast<T*>(r_comp);
	}

	// searches component tree for specific component of hashed string
	template <typename T>
	T* get_component(std::string id)
	{
		auto hashed_str = std::hash<std::string>{}(id);
		auto r_comp = components_.search(hashed_str);

		if (!r_comp)
			Logger::error("could not find component, str id = " + id, Logger::HIGH);

		return static_cast<T*>(r_comp);
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
		T* c(new T(std::forward<TArgs>(args)...));
		components_.insert(get_component_id<T>(), c);
		return c;
	}

	// adds component to splay tree using a hashed string
	template<typename T, typename... TArgs>
	T* add_id_component(std::string id, TArgs&&... args)
	{
		T* c(new T(std::forward<TArgs>(args)...));
		auto hashed_str = std::hash<std::string>{}(id);
		components_.insert(hashed_str, c);
		return c;
	}

	// adds component to splay tree using a hashed string
	template<typename T, typename... TArgs>
	T* add_id_component(std::size_t id, TArgs&&... args)
	{
		T* c(new T(std::forward<TArgs>(args)...));
		components_.insert(id, c);
		return c;
	}

	// adds component to splay tree and treats it like an array, can have two of the same component in an entity
	template<typename T, typename... TArgs>
	T* push_back_component(TArgs&&... args)
	{
		T* c(new T(std::forward<TArgs>(args)...));
		components_.insert(c);
		return c;
	}

	// gets array of components
	std::vector<IComponent*> get_component_list()
	{
		return components_.get_ordered_list();
	}	

	// delete all components and children
	void clear()
	{
		children_.clear();
		components_.clear();
	}
};
