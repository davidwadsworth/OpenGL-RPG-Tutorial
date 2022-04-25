#include "game.h"
#include "component_array.h"
#include "component_texunit.h"
#include "component_trigger_input_gamestate_overworld.h"
#include "component_trigger_input_gamestate_house.h"
#include "component_trigger_input_index.h"

// game global variables
Component::Trigger::Input::IGameState* Game::curr_state = nullptr;
std::string Game::next_state_ = "none";
GLuint Game::width = 800u;
GLuint Game::height = 600u;
GLfloat Game::delta_time = 0.0f;
Entity* Game::global = new Entity();
bool Game::exit = false;
glm::vec2 Game::removed(static_cast<int>(Game::width)* -2.0f - 1.0f, static_cast<int>(Game::height)* -2.0f - 1.0f);

// EC reference counting
long long IComponent::count = 0ll;
long long Entity::count = 0ll;

void Game::init(Entity* game)
{	
	auto& ctigs_overworld = *game->add_id_ct_input<Component::Trigger::Input::GameState::Overworld>("overworld");
	ctigs_overworld.execute(game);

	auto& ctigs_house = *game->add_id_ct_input<Component::Trigger::Input::GameState::House>("house");
	ctigs_house.execute(game);

	Game::global->add_id_component<Component::KeyboardArray>("keyboard");
	auto& cti_index = *Game::global->add_id_ct_input<Component::Trigger::Input::Index>("index", "index.json");
	cti_index.execute(Game::global);
}

void Game::set_next_state(std::string state)
{
	Game::next_state_ = state;
}

void Game::check_new_state(Entity* game)
{
	if (next_state_ == "none")
		return;

	Component::Trigger::Input::IGameState* c_next_state = game->get_component<Component::Trigger::Input::IGameState>(Game::next_state_);
	c_next_state->init();

	if (curr_state)
		Game::curr_state->destroy();
	
	Game::curr_state = c_next_state;
	next_state_ = "none";
}
