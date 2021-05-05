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

/*
Source code for episode 14 of Build Your Own RPG series

@author David Wadsworth
*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void APIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity,
    GLsizei length, const char* message, const void* userParam);
void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode);

int main()
{
    if (!glfwInit())
        Logger::error("failed to initialize glfw!", 1);

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
        Logger::error("failed to initialize glad!", 1);

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

    auto game = new Entity();

    // init game
    Game::init(game);

    auto current_state = game->get_child("overworld");

    auto overworld_game_objs = current_state->get_component_list();

    for (auto obj : overworld_game_objs)
        static_cast<Component::Trigger::In*>(obj)->execute(current_state);

    // set up used game objects
    auto engine = current_state->get_child("engine");
    auto& render_systems = *engine->get_component<Component::SystemVector>("render");
    auto& update_systems = *engine->get_component<Component::SystemVector>("update");

    auto& c_renderer = *current_state->get_child("renderer")->get_component<Component::Renderer>();

    Logger::message("Entities Created: " + std::to_string(Entity::count));
    Logger::message("Components Created: " + std::to_string(Comp::count));
    
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
        Logger::warning("Entity Memory Leak: " + Entity::count, 0);
    if (Comp::count)
        Logger::warning("Component Memory Leak: " + Comp::count, 0);

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode)
{
    if (key >= 0 && key < MAX_KEYS)
    {
        // update our global keyboard object
        if (action == GLFW_PRESS)
            Game::keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            Game::keys[key] = GL_FALSE;
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

    std::string str;
    int sev{};

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             str += "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   str += "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: str += "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     str += "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     str += "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           str += "Source: Other"; break;
    } 

    str += "\n";

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               str += "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: str += "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  str += "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         str += "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         str += "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              str += "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          str += "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           str += "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               str += "Type: Other"; break;
    }

    str += "\n";

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         sev = 2; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       sev = 1; break;
    case GL_DEBUG_SEVERITY_LOW:          sev = 0; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: sev = 0; break;
    } 

    Logger::error("OpenGL error (" + id + std::string("): ") + std::string(message) + "\n" + str, sev);
}