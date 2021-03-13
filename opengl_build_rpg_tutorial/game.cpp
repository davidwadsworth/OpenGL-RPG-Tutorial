#include "game.h"
#include "entity.h"
#include "component.h"

// game global variables
GLuint Game::width = 800u;
GLuint Game::height = 600u;
std::array<bool, MAX_KEYS> Game::keys{0};

// EC reference counting
long long Comp::count = 0ll;
long long Entity::count = 0ll;
