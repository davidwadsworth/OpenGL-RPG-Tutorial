#pragma once
#include "component_trigger.h"

enum class NPATH
{
	STAY = 0,
	CHILD_1 = 1,
	CHILD_2 = 2,
	CHILD_3 = 3
};



class INavigator
{
public:
	virtual NPATH navigate() = 0;
};



