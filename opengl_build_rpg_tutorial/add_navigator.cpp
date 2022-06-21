#include "add_navigator.h"
#include "navigator_optionboxh.h"
#include "navigator_optionboxv.h"
#include "navigator_textbox.h"

INavigator* add_navigator(nlohmann::json json, std::string name, Entity* gamestate)
{
	if (name == "textbox")
		return new Navigator::Textbox();
	if (name == "optionboxv")
		return new Navigator::OptionBoxV(gamestate, json);
	if (name == "optionboxh")
		return new Navigator::OptionBoxH(gamestate, json);

	Logger::error("navigor of name " + std::string(name) + " not found", Logger::HIGH);
	return nullptr;
}
