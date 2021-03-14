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
#include "component_system_render_camera_draw.h"
#include "component_system_update_camera.h"
#include "component_controller_keyboard.h"
#include "component_system_update_move.h"
#include "component_system_update_animation.h"
#include "component_system_update_animate_move.h"

/*
Source code for episode 10 of Build Your Own RPG series

@author David Wadsworth
*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void APIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity,
    GLsizei length, const char* message, const void* userParam);
void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode);

constexpr Rect SRC{ 0.0f, 0.0f, 64.0f, 64.0f };
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

    // set up engine, will be its own thing soon enough
    std::vector<Component::System*> render_systems;
    std::vector<Component::System*> update_systems;

    // set up entities and their components
    
    // load in used shaders
    auto vs_file_name = "resources/shaders/sprite.vs";
    auto fs_file_name = "resources/shaders/sprite.fs";

    auto shaders = new Entity();

    auto& c_shader = *shaders->add_component<Component::Shader>();
    c_shader.load(vs_file_name, fs_file_name);

    // set up camera
    c_shader.use();
    auto projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
    c_shader.set_mat4("projection", projection);

    // load in used textures
    auto textures = new Entity();

    auto flesh_tex_name = "resources/images/flesh_full.png";
    auto grass_tex_name = "resources/images/grass.png";

    auto& c_flesh_tex = *textures->add_component<Component::Texture>();
    c_flesh_tex.load(flesh_tex_name);

    auto& c_grass_tex = *textures->push_back_component<Component::Texture>();
    c_grass_tex.load(grass_tex_name);

    // create a renderer object and input appropriate attribute sizes and max amount of sprites on screen at once
    // 2 = pos, 2 = coords
    auto renderer = new Entity();
    auto& c_renderer = *renderer->add_component<Component::Renderer>(std::vector<GLuint>{ 2,2 }, MAX_SPRITES);

    // set up controller
    auto controller = new Entity();
    auto& c_cont_keyboard = *controller->add_component<ComponentController::Keyboard>();

    // set up camera
    auto camera = new Entity();
    auto& c_cam_transform = *camera->add_component<Component::Transform>(0.0f, 0.0f, (GLfloat)ROWS * 64.0f); // position = (0, 0) width/height = 32 tiles * 64 length of tile

    // set up tile map
    auto tile_map = new Entity();

    auto& c_tmap_material = *tile_map->add_component<Component::Material>(c_grass_tex, c_shader, 1);

    // how many total grass to draw
    auto total_grass = COLS * ROWS;

    auto tiles = new Entity();
    tile_map->push_back_child(tiles);

    // set up tiles
    for (auto i = 0; i < total_grass; ++i)
    {
        Rect grass_dest
        {
            (i % COLS) * 64.0f,  // finds place in column and multiplies by sprite width
            (i / COLS) * 64.0f,  // finds place in row and multiples by sprite height
            64.0f, 64.0f
        };
        auto& c_tile_transform = *tiles->push_back_component<Component::Transform>(grass_dest);
        auto& c_tile_src = *tiles->push_back_component<Component::Src>(SRC);
        auto& c_tile_dest = *tiles->push_back_component<Component::Dest>();
        auto csr_tile_dynamic_draw = tiles->push_back_component<ComponentSystemRender::CameraDraw>(c_renderer, c_tile_src, c_tile_dest, c_tmap_material, c_tile_transform, c_cam_transform);
        render_systems.push_back(csr_tile_dynamic_draw);
    }

    // set up player and it's components
    auto player = new Entity();

    auto& c_pla_transform = *player->add_component<Component::Transform>((GLfloat)Game::width, (GLfloat)Game::height, 64.0f);
    auto& c_pla_src = *player->add_component<Component::Src>(SRC);
    auto& c_pla_dest = *player->add_component<Component::Dest>(); 
    auto& c_pla_material = *player->add_component<Component::Material>(c_flesh_tex, c_shader, 0);

    auto csr_pla_dynamic_draw = player->add_component<ComponentSystemRender::CameraDraw>(c_renderer, c_pla_src, c_pla_dest, c_pla_material, c_pla_transform, c_cam_transform);
    auto csu_pla_camera = player->add_component<ComponentSystemUpdate::Camera>(c_pla_transform, c_cam_transform);
    auto csu_pla_move = player->add_component<ComponentSystemUpdate::Move>(c_pla_transform, c_cont_keyboard);

    auto csu_pla_animation = player->add_component<ComponentSystemUpdate::Animation>(4, c_pla_src);
    auto csu_pla_animate_move = player->add_component<ComponentSystemUpdate::AnimateMove>(c_cont_keyboard, *csu_pla_animation);

    // set up flesh animations
    std::string anims[] = {
        "idle down",
        "idle up",
        "idle right",
        "idle left",
        "walk down",
        "walk up",
        "walk right",
        "walk left"
    };

    auto anim_i = 0u;
    auto animation = new Entity();

    auto flesh_tex_cols = c_flesh_tex.width / 64u;

    for (auto i = 0; i < 4; ++i)
    {
        Rect rect{
            (GLfloat)(anim_i % flesh_tex_cols) * 64.0f,
            (GLfloat)(anim_i / flesh_tex_cols) * 64.0f,
            64.0f,
            64.0f
        };

        auto idle = animation->push_back_component<Component::Src>(rect);
        csu_pla_animation->add(anims[anim_i++], Anim{idle});
    }

    for (auto i = 0; i < 4; ++i)
    {
        Rect rect{
            (GLfloat)((anim_i + i) % flesh_tex_cols) * 64.0f,
            (GLfloat)((anim_i + i) / flesh_tex_cols) * 64.0f,
            64.0f,
            64.0f
        };

        Rect rect2{
            (GLfloat)((anim_i + i + 1) % flesh_tex_cols) * 64.0f,
            (GLfloat)((anim_i + i + 1) / flesh_tex_cols) * 64.0f,
            64.0f,
            64.0f
        };

        auto walk_1 = animation->push_back_component<Component::Src>(rect);
        auto walk_2 = animation->push_back_component<Component::Src>(rect2);
        csu_pla_animation->add(anims[anim_i++], Anim{ walk_1, walk_2 });
    }

    render_systems.push_back(csr_pla_dynamic_draw);
    update_systems.push_back(csu_pla_move);
    update_systems.push_back(csu_pla_camera);
    update_systems.push_back(csu_pla_animate_move);
    update_systems.push_back(csu_pla_animation);

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
    delete player;
    delete camera;
    delete tile_map;
    delete shaders;
    delete textures;
    delete renderer;
    delete controller;
    delete animation;

    glfwTerminate();

    if (Entity::count)
    {
        std::cerr << "Entity Memory Leak: " << Entity::count << std::endl;
        return -1;
    }

    if (Comp::count)
    {
        std::cerr << "Component Memory Leak: " << Comp::count << std::endl;
        return -1;
    }

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
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