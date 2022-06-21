#pragma once
#include <string>
#include "json.hpp"
#include "entity.h"
#include "navigator.h"

INavigator* add_navigator(nlohmann::json json, std::string name, Entity* gamestate);
