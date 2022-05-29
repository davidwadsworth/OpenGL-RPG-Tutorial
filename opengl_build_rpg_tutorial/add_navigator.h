#pragma once
#include "navigator_textbox.h"
#include <string>
#include "json.hpp"
#include "navigator_optionboxh.h"
#include "navigator_optionboxv.h"
#include "entity.h"

INavigator* add_navigator(nlohmann::json json, Entity* gamestate)
{
	std::string name = json["name"];

	if (name == "textbox")
		return new Navigator::Textbox();
	if (name == "optionboxv")
		return new Navigator::OptionBoxV(gamestate, json);
	if (name == "optionboxh")
		return new Navigator::OptionBoxH(gamestate, json);

	Logger::error("navigor of name " + std::string(name) + " not found", Logger::HIGH);
	return nullptr;
}
