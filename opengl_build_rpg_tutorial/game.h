#pragma once
#include <glad/glad.h>
#include <array>
#include <unordered_map>
#include <glm/vec2.hpp>
#include "entity.h"

/*
defines key characteristics of the game and its state.

@author David Wadsworth
*/


class Game
{
	static std::string next_state_;
public:
	Game() = delete;
	static GLuint width;
	static GLuint height;
	static GLfloat delta_time;
	static Entity* global;
	static bool exit;
	static glm::vec2 removed;

	void init(Entity* game);
	void render();
	void pathway();
	void update();
};