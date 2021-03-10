#pragma once
/*
 stores generics and data structures for systems to manipulate and display on screen
 @author David Wadsworth
*/
#include <iostream>

struct Comp
{
	static long long component_count;

	Comp()
	{
		component_count++;
	}
	
	virtual ~Comp()
	{
		component_count--;
	}
};