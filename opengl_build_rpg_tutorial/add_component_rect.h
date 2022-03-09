#pragma once
#include "json.hpp"
#include "entity.h"
#include "component_rect.h"

/*
Convert flatfile info (collision info, action info, etc...) into usable game object info.

@author David Wadsworth
*/

Component::Rectang* add_component_rect(Entity* entity, Entity* gamestate, nlohmann::json info_json);

