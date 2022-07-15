#pragma once
/*
 Stores generics and data structures for systems to manipulate and display on screen

 @author David Wadsworth
*/

struct IComponent
{
	static long long count; // count component references

	IComponent() {++count;}
	virtual ~IComponent() { --count; } // Make sure if overloaded to call ~Comp()

	// delete all functions that could possibly copy one component onto another
	IComponent(const IComponent&) = delete;
	IComponent(IComponent&&) = delete;
	IComponent& operator=(const IComponent&) = delete;
	IComponent& operator=(IComponent&&) = delete;

};