#pragma once
#include <vector>
#include "component.h"
#include <memory>
#include <array>
#include "glad/glad.h"
#include <iostream>
#include "splay_tree.h"

using component_id = std::size_t;

inline component_id get_new_component_type_id()
{
	static component_id last_id = 0u;
	return ++last_id;
}

// depending on how many components there are creates a seperate get_component_type_if for each, thus giving the illusion of unique id
template <typename T> inline component_id get_component_type_id() noexcept
{
	// assert that it inherits Comp
	static_assert (std::is_base_of<Comp, T>::value, "");
	static component_id type_id = get_new_component_type_id();
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
	DataStructures::SplayTree<Comp> components_;
	DataStructures::SplayTree<Entity> children_;
public:	
	static long long count; // count entity references
	
	Entity()
		: components_(), children_()
	{
		++count;
	}

	~Entity()
	{
		components_.~SplayTree();
		children_.~SplayTree();
		--count;
	}

	// delete all functions that could possibly copy one entity onto another
	Entity(const Entity&) = delete;
	Entity(Entity&&) = delete;
	Entity& operator=(const Entity&) = delete;
	Entity& operator=(Entity&&) = delete;

	Entity* get_child(GLuint pos)
	{
		return children_.search(pos);
	}

	Entity* get_child(std::string str)
	{
		return children_.search((int)std::hash<std::string>{}(str));
	}

	std::vector<Entity*> get_child_list()
	{
		return children_.get_ordered_list();
	}

	bool has_child(std::string str)
	{
		return get_child(str);
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
	Entity* add_child(Entity* entity, std::string str)
	{
		return children_.insert(std::hash<std::string>{}(str), entity);
	}

	// gets component of matching unique id, throws if not there
	template<typename T> T* get_component()
	{
		return static_cast<T*>(components_.search(get_component_type_id<T>()));
	}

	// searches component tree for templated component at pos
	template<typename T> T* get_component(int pos)
	{
		return static_cast<T*>(components_.search(pos));
	}

	// checks if unique id of component is located in component tree
	template <typename T> bool has_component()
	{
		return components_.search(get_component_type_id<T>());
	}

	// adds component to splay tree and gives it a uniquely id, can't have two of the same component per entity
	template <typename T, typename... TArgs> T* add_component(TArgs&&... m_args)
	{
		T* c(new T(std::forward<TArgs>(m_args)...));
		return static_cast<T*>(components_.insert(get_component_type_id<T>(), c));
	}

	// adds component to splay tree and treats it like an array, can have two of the same component in an entity
	template <typename T, typename... TArgs> T* push_back_component(TArgs&&... m_args)
	{
		T* c(new T(std::forward<TArgs>(m_args)...));
		return static_cast<T*>(components_.insert(c));
	}

	// gets array of components
	std::vector<Comp*> get_component_list()
	{
		return components_.get_ordered_list();
	}	
};
