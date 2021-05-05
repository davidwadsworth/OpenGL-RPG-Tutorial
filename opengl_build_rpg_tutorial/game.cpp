#include "game.h"
#include "entity.h"
#include "component.h"
#include "component_trigger_input_gamestate_overworld.h"

// game global variables
GLuint Game::width = 800u;
GLuint Game::height = 600u;
GLfloat Game::delta_time = 0.0f;
std::array<bool, MAX_KEYS> Game::keys{0};
Entity* Game::global = new Entity();
bool Game::exit = false;

// EC reference counting
long long Comp::count = 0ll;
long long Entity::count = 0ll;

void Game::init(Entity* game)
{
	auto& ctigs_overworld = *game->add_component<Component::Trigger::Input::GameState::Overworld>("overworld");
	ctigs_overworld.execute(game);
}
