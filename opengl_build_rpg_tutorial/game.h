#pragma once
#include <glad/glad.h>
#include <array>
#include <unordered_map>
#include <glm/vec2.hpp>
#include "entity.h"
#include "component_system.h"
#include "component_pathway.h"

/*
defines key characteristics of the game and the state it is in.

@author David Wadsworth
*/


class Game
{
	std::vector<Component::ISystem*> render_;
	std::vector<Component::ISystem*> update_;
	std::vector<Component::Pathway*> pathway_;

	Component::Pathway* global_pathway_;
	Entity* e_game_;
public:
	static GLuint width;
	static GLuint height;
	static GLfloat delta_time;
	static Entity* global;
	static bool exit;
	
	void init(std::string begin_state, std::string index_path);
	void render();

	void pathway();
	void update();
};