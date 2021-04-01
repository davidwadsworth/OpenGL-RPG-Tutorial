#include "game.h"
#include "entity.h"
#include "component.h"
#include "component_trigger_input_gamestate_overworld.h"

// game global variables
GLuint Game::width = 800u;
GLuint Game::height = 600u;
std::array<bool, MAX_KEYS> Game::keys{0};

EntityMap Game::global_objects;
Entity* Game::game = new Entity();

// EC reference counting
long long Comp::count = 0ll;
long long Entity::count = 0ll;

void Game::init()
{
	auto& ctdigs_overworld = *game->add_component<Component::Trigger::Input::GameState::Overworld>("overworld");
	ctdigs_overworld.execute(global_objects);
}
