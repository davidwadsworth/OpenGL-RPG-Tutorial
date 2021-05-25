#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "game.h"
#include "component_system.h"
#include "component_trigger_input.h"
#include "component_vector.h"
#include "component_renderer.h"
#include "logger.h"
#include "component_array.h"
#include <sstream>

/*
Source code for episode 15 of Build Your Own RPG series

@author David Wadsworth
*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void APIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity,
    GLsizei length, const char* message, const void* userParam);
void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode);

int main()
{
    auto game = new Entity();

    // init game
    Game::init(game);

    if (!glfwInit())
        Logger::error("failed to initialize glfw!", Logger::HIGH);

    // opengl version = major.minor
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    // use the deprecated functions? (CORE_PROFILE = yes, COMPATABILITY = no)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // show debug output with glfw
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    auto window = glfwCreateWindow(Game::width, Game::height, "Build Your Own RPG", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    // make the window resizable and scale the renderer
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // set up so keyboard can interact with window
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        Logger::error("failed to initialize glad!", Logger::HIGH);

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_output, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    // normalize window to work on other devices
    glViewport(0, 0, Game::width, Game::height);

    // set up alpha channel to display images beneath it.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    auto current_state = game->get_child("overworld");

    auto overworld_game_objs = current_state->get_component_list();

    for (auto obj : overworld_game_objs)
        static_cast<Component::Trigger::IInput*>(obj)->execute(current_state);

    // set up used game objects
    auto engine = current_state->get_child("engine");
    auto& render_systems = *engine->get_component<Component::SystemVector>("render");
    auto& update_systems = *engine->get_component<Component::SystemVector>("update");
    auto& trigger_systems = *engine->get_component<Component::TriggerVector>("trigger");

    auto& c_renderer = *current_state->get_child("renderer")->get_component<Component::Renderer>();

    Logger::message("Entities Created: " + std::to_string(Entity::count));
    Logger::message("Components Created: " + std::to_string(IComponent::count));
    
    GLfloat last_frame = 0.0f;

    // game loop
    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        auto current_frame = static_cast<GLfloat>(glfwGetTime());
        Game::delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // clear screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // make updates to live entities
        for (auto u : update_systems)
            u->execute();

        c_renderer.begin();
        
        // make draw calls to renderer
        for (auto r : render_systems)
            r->execute();
            
        c_renderer.end();

        if (Game::exit)
            glfwSetWindowShouldClose(window, GL_TRUE);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    render_systems.clear();
    update_systems.clear();

    // delete entities and their components
    delete Game::global;
    delete game;
    
    glfwTerminate();

    if (Entity::count)
        Logger::warning("Entity Memory Leak: " + Entity::count, Logger::HIGH);
    if (IComponent::count)
        Logger::warning("Component Memory Leak: " + IComponent::count, Logger::HIGH);

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode)
{
    static Component::KeyboardArray& keys = *Game::global->get_child("keyboard")->get_component<Component::KeyboardArray>();
    
    if (key >= 0 && key < MAX_KEYS)
    {
        // update our global keyboard object
        if (action == GLFW_PRESS)
            keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            keys[key] = GL_FALSE;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void APIENTRY gl_debug_output(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::stringstream ss;
    ss << "OpenGL error (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             ss << "SOURCE: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   ss << "SOURCE: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: ss << "SOURCE: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     ss << "SOURCE: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     ss << "SOURCE: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           ss << "SOURCE: Other"; break;
    } ss << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               ss << "TYPE: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: ss << "TYPE: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  ss << "TYPE: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         ss << "TYPE: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         ss << "TYPE: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              ss << "TYPE: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          ss << "TYPE: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           ss << "TYPE: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               ss << "TYPE: Other"; break;
    } ss << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         ss << "GL SEVERITY: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       ss << "GL SEVERITY: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          ss << "GL SEVERITY: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: ss << "GL SEVERITY: notification"; break;
    }

    Logger::error(ss.str(), Logger::HIGH);
}