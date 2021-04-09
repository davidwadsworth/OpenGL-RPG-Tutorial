#pragma once
#include <glad/glad.h>
#include <array>
#include "entity.h"
#include <unordered_map>

/*
defines key characteristics of the game and its state.

@author David Wadsworth
*/

constexpr auto MAX_KEYS = 1024u;

#define EntityMap std::unordered_map<std::string, std::unique_ptr<Entity>>

class Game
{
public:
	Game() = delete;
	static GLuint width;
	static GLuint height;
	static GLfloat delta_time;
	static std::array<bool, MAX_KEYS> keys;
	static EntityMap global_objects;
	static Entity* game;

	static void init();
};