#include "game.h"
#include "component_array.h"
#include "component_texunit.h"
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
	Game::global->add_id_component<Component::KeyboardArray>("keyboard");
	auto& cc_keyboard = *Game::global->add_id_component<Component::Controller::Keyboard>("controller");
	auto& cti_index = *Game::global->add_id_ct_input<Component::Trigger::Input::Index>("index", "index.json");
	cti_index.execute(Game::global);

	auto keyboard_controller_json = cti_index->get_component<Component::Json>("controller_keyboard")->json;

	cc_keyboard.set_keys(keyboard_controller_json);
}

void Game::render()
{
}

void Game::update()
{
}

void Game::pathway()
{
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
