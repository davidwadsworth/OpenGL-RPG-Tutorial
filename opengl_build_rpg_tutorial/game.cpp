#include "game.h"
#include "component_array.h"
#include "component_texunit.h"
// game global variables
GLuint Game::width = 800u;
GLuint Game::height = 600u;
GLfloat Game::delta_time = 0.0f;
Entity* Game::global = new Entity();
bool Game::exit = false;

// EC reference counting
long long IComponent::count = 0ll;
long long Entity::count = 0ll;


void Game::init(std::string begin_state, std::string index_path)
{
	e_game_ = new Entity();
	Game::global->add_id_component<Component::KeyboardArray>("keyboard");
	auto& cc_keyboard = *Game::global->add_id_component<Component::Controller::Keyboard>("controller");

	auto keyboard_controller_json = cti_index->get_component<Component::Json>("controller_keyboard")->json;

	cc_keyboard.set_keys(keyboard_controller_json);
}

void Game::render()
{
	for (auto r : render_)
		r->execute();
}

void Game::update()
{
	for (auto u : update_)
		u->execute();
}

void Game::pathway()
{
	for (auto p : pathway_)
		p->navigate();

	global_pathway_->navigate();
}
