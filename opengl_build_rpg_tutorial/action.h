#pragma once
#include "json.hpp"

/*
A storage object used with passing json based action instructions.
This is mostly used in junction with textbox and worldly based instructions.

@author David Wadsworth
*/

class Act
{
public:
	nlohmann::json action;
};