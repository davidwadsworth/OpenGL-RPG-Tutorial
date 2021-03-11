#pragma once
/*
 Stores generics and data structures for systems to manipulate and display on screen

 @author David Wadsworth
*/

struct Comp
{
	static long long count; // count component references

	Comp() {++count;}
	virtual ~Comp() { --count; } // Make sure if overloaded to call ~Comp() else it will falsely acuse you of a memory leak

	// delete all functions that could possibly copy one component onto another
	Comp(const Comp&) = delete;
	Comp(Comp&&) = delete;
	Comp& operator=(const Comp&) = delete;
	Comp& operator=(Comp&&) = delete;
};