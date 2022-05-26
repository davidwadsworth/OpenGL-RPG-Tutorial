#pragma once
#include "navigator_textbox.h"
#include <string>

INavigator* create_navigator(std::string name, std::vector<Component::ITrigger*> triggers)
{
	if (name == "textbox")
		return new Navigator::Textbox(triggers);

	Logger::error("navigor of name " + std::string(name) + " not found", Logger::HIGH);
	return nullptr;
}
