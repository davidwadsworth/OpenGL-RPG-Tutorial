#pragma once
#include <glad/glad.h>
#include <array>

/*
defines key characteristics of the game and its state.

@author David Wadsworth
*/

constexpr auto MAX_KEYS = 1024u;

class Game
{
public:
	Game() = default;
	static GLuint width, height;
	static std::array<bool, MAX_KEYS> keys;
};