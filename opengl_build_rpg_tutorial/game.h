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

class Game
{
public:
	Game() = delete;
	static GLuint width;
	static GLuint height;
	static GLfloat delta_time;
	static std::array<bool, MAX_KEYS> keys;
	static Entity* global;

	static void init(Entity* game);
};