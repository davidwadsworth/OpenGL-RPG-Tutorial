#pragma once
#include <glad/glad.h>
#include <array>
#include "entity.h"
#include <unordered_map>
#include <glm/vec2.hpp>

/*
defines key characteristics of the game and its state.

@author David Wadsworth
*/


class Game
{
public:
	Game() = delete;
	static GLuint width;
	static GLuint height;
	static GLfloat delta_time;
	static Entity* global;
	static bool exit;
	static glm::vec2 removed;

	static void init(Entity* game);
};