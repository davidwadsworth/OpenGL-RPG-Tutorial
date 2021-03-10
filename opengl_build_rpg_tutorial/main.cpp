#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "component_shader.h"
#include "component_texture.h"
#include <glm/ext/matrix_clip_space.hpp>
#include "component_renderer.h"
#include "component_material.h"
#include "game.h"
#include "entity.h"
#include "component_transform.h"
#include "component_system.h"
#include "component_system_render_dynamic_draw.h"
#include "component_system_update_camera.h"

/*
Source code for episode 7 of Build Your Own RPG series

@author David Wadsworth
*/


void processInput(GLFWwindow* window, Component::Transform& transform);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
    GLsizei length, const char* message, const void* userParam);

constexpr Rect SRC{ 0.0f, 0.0f, 64.0f, 64.0f };
constexpr GLfloat SPEED = 4.0f;
constexpr GLint COLS = 32;
constexpr GLint ROWS = 32;
constexpr GLuint MAX_SPRITES = 255u;

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "failed to initialize glad!" << std::endl;
        return -1;
    }

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    // normalize window to work on other devices
    glViewport(0, 0, Game::width, Game::height);

    // set up alpha channel to display images beneath it.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // set up engine, will be its own thing soon enough
    std::vector<Component::System*> render_systems;
    std::vector<Component::System*> update_systems;

    // set up entities and their components
    
    // load in used shaders
    auto vs_file_name = "resources/shaders/sprite.vs";
    auto fs_file_name = "resources/shaders/sprite.fs";

    auto shader = Component::Shader();
    shader.load(vs_file_name, fs_file_name);

    // set up camera
    shader.use();
    auto projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
    shader.set_mat4("projection", projection);

    // load in used textures
    auto flesh_tex_name = "resources/images/flesh.png";
    auto grass_tex_name = "resources/images/grass.png";

    auto flesh_tex = Component::Texture();
    flesh_tex.load(flesh_tex_name);

    auto grass_tex = Component::Texture();
    grass_tex.load(grass_tex_name);

    // create a renderer object and input appropriate attribute sizes and max amount of sprites on screen at once
    // 2 = pos, 2 = coords
    auto renderer = Component::Renderer({ 2, 2 }, MAX_SPRITES);

    // set up camera
    auto camera = new Entity();
    auto& c_cam_transform = *camera->add_component<Component::Transform>(0.0f, 0.0f, (GLfloat)ROWS * 64.0f); // position = (0, 0) width/height = 32 tiles * 64 length of tile

    // set up tile map
    auto tile_map = new Entity();

    auto& c_tmap_material = *tile_map->add_component<Component::Material>(grass_tex, shader, 1);

    // how many columns of grass
    auto grass_cols = COLS;

    // how many rows of grass
    auto grass_rows = ROWS;

    // how many total grass to draw
    auto total_grass = grass_cols * grass_rows;

    auto tiles = new Entity();
    tile_map->push_back_child(tiles);

    // set up tiles
    for (auto i = 0; i < total_grass; ++i)
    {
        Rect grass_dest
        {
            (i % grass_cols) * 64.0f,  // finds place in column and multiplies by sprite width
            (i / grass_cols) * 64.0f,  // finds place in row and multiples by sprite height
            64.0f, 64.0f
        };
        auto& c_tile_transform = *tiles->push_back_component<Component::Transform>(grass_dest);
        auto& c_tile_src = *tiles->push_back_component<Component::Src>(SRC);
        auto& c_tile_dest = *tiles->push_back_component<Component::Dest>();
        auto csr_tile_dynamic_draw = tiles->push_back_component<ComponentSystemRender::DynamicDraw>(renderer, c_tile_src, c_tile_dest, c_tmap_material, c_tile_transform, c_cam_transform);
        render_systems.push_back(csr_tile_dynamic_draw);
    }

    // set up player and it's components
    auto player = new Entity();

    auto& c_pla_transform = *player->add_component<Component::Transform>((GLfloat)Game::width, (GLfloat)Game::height, 64.0f, 2.0f);
    auto& c_pla_src = *player->add_component<Component::Src>(SRC); // src is full image, dest is set up during dynamic draw
    auto& c_pla_dest = *player->add_component<Component::Dest>(); 
    auto& c_pla_material = *player->add_component<Component::Material>(flesh_tex, shader, 0);
    auto csr_pla_dynamic_draw = player->add_component<ComponentSystemRender::DynamicDraw>(renderer, c_pla_src, c_pla_dest, c_pla_material, c_pla_transform, c_cam_transform);
    auto csu_pla_camera = player->add_component<ComponentSystemUpdate::Camera>(c_pla_transform, c_cam_transform);

    render_systems.push_back(csr_pla_dynamic_draw);
    update_systems.push_back(csu_pla_camera);

    // game loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window, c_pla_transform);

        // clear screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // make updates to live entities
        for (auto u : update_systems)
            u->execute();

        renderer.begin();

        // make draw calls to renderer
        for (auto r : render_systems)
            r->execute();

        renderer.end();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete player;
    delete camera;
    delete tile_map;
    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window, Component::Transform& transform)
{ 

    // move character around screen
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        transform.y-= SPEED;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        transform.y+= SPEED;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        transform.x+= SPEED;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        transform.x-= SPEED;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // exit on escape key press
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void APIENTRY glDebugOutput(GLenum source,
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