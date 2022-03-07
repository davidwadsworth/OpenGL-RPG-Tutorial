#pragma once
#include "json.hpp"
#include "entity.h"
#include "component_rect.h"

Component::Rectang* add_component_rect(Entity* entity, Entity* gamestate, nlohmann::json info_json);

