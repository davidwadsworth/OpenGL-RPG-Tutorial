#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "game.h"
#include "component_system.h"
#include "component_trigger_input.h"
#include "component_vector.h"
#include "component_renderer.h"

/*
Source code for episode 10 of Build Your Own RPG series

@author David Wadsworth
*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void APIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity,
    GLsizei length, const char* message, const void* userParam);
void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode);

int main()
{
    if (!glfwInit())
    {
        std::cerr << "failed to initialize glfw!" << std::endl;
        return -1;
    }

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
    {
        std::cerr << "failed to initialize glad!" << std::endl;
        glfwTerminate();
        return -1;
    }

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

    // init game
    Game::init();

    EntityMap local_objects;

    auto overworld_game_objs = Game::global_objects["overworld"]->get_component_list();

    for (auto obj : overworld_game_objs)
        static_cast<Component::Trigger::In*>(obj)->execute(local_objects);

    // set up used game objects
    auto engine = local_objects["engine"].get();
    auto& render_systems = *engine->get_component<Component::Vector<Component::Sys*>>("render");
    auto& update_systems = *engine->get_component<Component::Vector<Component::Sys*>>("update");

    auto& c_renderer = *local_objects["renderer"]->get_component<Component::Renderer>();

    std::cout << "Entities Created: " << Entity::count << std::endl;
    std::cout << "Components Created: " << Comp::count << std::endl;

    // game loop
    while (!glfwWindowShouldClose(window))
    {
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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    render_systems.clear();
    update_systems.clear();

    // delete entities and their components
    local_objects.clear();
    Game::global_objects.clear();
    delete Game::game;
    
    glfwTerminate();

    if (Entity::count)
        std::cerr << "Entity Memory Leak: " << Entity::count << std::endl;
    if (Comp::count)
        std::cerr << "Component Memory Leak: " << Comp::count << std::endl;

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

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}