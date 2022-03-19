#pragma once
#include <glad/glad.h>
#include <array>
#include <unordered_map>
#include <glm/vec2.hpp>
#include "component_trigger_input_gamestate.h"
#include "entity.h"

/*
defines key characteristics of the game and its state.

@author David Wadsworth
*/


enum class GameStateEn : std::size_t {
	none,
	overworld,
	house
};

class Game
{
	static Component::Trigger::Input::IGameState* prev_state_;
	static GameStateEn next_state_;
public:
	Game() = delete;
	static GLuint width;
	static GLuint height;
	static GLfloat delta_time;
	static Component::Trigger::Input::IGameState* curr_state;
	static Entity* global;
	static bool exit;
	static glm::vec2 removed;

	static void init(Entity* game);
	static void set_next_state(GameStateEn state);
	static void check_new_state(Entity* game);
};