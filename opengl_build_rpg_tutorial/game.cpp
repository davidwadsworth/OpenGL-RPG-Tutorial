#include "game.h"
#include "entity.h"
#include "component.h"
#include "component_trigger_input_gamestate_overworld.h"
#include "component_array.h"
#include "component_texunit.h"

// game global variables
Component::Trigger::Input::IGameState* Game::prev_state_ = nullptr,
	* Game::curr_state = nullptr;
std::string next_state = "none";
Component::Trigger::Input::IGameState* Game::curr_state = nullptr;
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

	Game::global->add_id_component<Component::KeyboardArray>("keyboard");
}

void Game::set_next_state(std::string state)
{
	next_state_ = state;
}

void Game::check_new_state(Entity* game)
{
	auto next_state = game->get_component<Component::Trigger::Input::IGameState>(next_state_);

	if (Game::prev_state_)
	{
		// if we are swapping from the current to the previous we don't need to unload the previous 
		// we can just swap.
		if (Game::prev_state_ == next_state)
		{
			auto e_temp = Game::curr_state;
			Game::curr_state = Game::prev_state_;
			prev_state_ = e_temp;
			return;
		}
		else
			Game::prev_state_->destroy();
	}

	next_state->init();
	Game::prev_state_ = Game::curr_state;
	Game::curr_state = next_state;
		
}
