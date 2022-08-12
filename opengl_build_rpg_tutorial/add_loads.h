#pragma once
#include "load_box.h"
#include "load_message.h"
#include "load_optionbox.h"
#include "load_switchstate.h"



void add_loads(std::unordered_map<std::string, ICommand*> map, std::vector<std::unique_ptr<ICommand>> created_commands)
{
	auto load_box = new Load::Box();
	auto load_message = new Load::Message();
	auto load_optionbox = new Load::OptionBox();
	auto load_switchstate = new Load::SwitchState();

	created_commands.push_back(std::unique_ptr<ICommand>(load_box));
	created_commands.push_back(std::unique_ptr<ICommand>(load_message));
	created_commands.push_back(std::unique_ptr<ICommand>(load_optionbox));
	created_commands.push_back(std::unique_ptr<ICommand>(load_switchstate));

	map["box"] = load_box;
	map["message"] = load_message;
	map["option_box"] = load_optionbox;
	map["box"] = load_switchstate;
}