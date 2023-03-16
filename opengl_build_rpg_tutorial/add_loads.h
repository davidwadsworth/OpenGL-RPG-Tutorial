#pragma once
#include "load_box.h"
#include "load_message.h"
#include "load_optionbox.h"
#include "load_switchstate.h"



void add_loads(std::unordered_map<std::string, std::unique_ptr<ICommand>> map)
{
	map["box"] = std::unique_ptr<ICommand>(new Load::Box());
	map["message"] = std::unique_ptr<ICommand>(new Load::Message());
	map["option_box"] = std::unique_ptr<ICommand>(new Load::OptionBox());
}