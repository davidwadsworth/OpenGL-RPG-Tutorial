#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "shader.h"
#include "texture.h"
#include <glm/ext/matrix_clip_space.hpp>
#include "renderer.h"
#include "material.h"

/*
Source code for episode 5 of Build Your Own RPG series

@author David Wadsworth
*/

void processInput(GLFWwindow* window);

Rect flesh_dest = { 5, 5, 64, 64 };
Rect flesh_dest2 = { 800 - 69, 600 - 69, 64, 64 };
Rect src = { 0,0,1,1 };

constexpr auto SPEED = 4.0f;

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

    auto window = glfwCreateWindow(800, 600, "Build Your Own RPG", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "failed to initialize glad!" << std::endl;
        return -1;
    }

    // normalize window to work on other devices
    glViewport(0, 0, 800, 600);

    // set up alpha channel to display images beneath it.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // load in used shaders
    auto vs_file_name = "resources/shaders/sprite.vs";
    auto fs_file_name = "resources/shaders/sprite.fs";

    auto shader = Shader();
    shader.load(vs_file_name, fs_file_name);

    // load in used textures
    auto flesh_tex_name = "resources/images/flesh.png";
    auto grass_tex_name = "resources/images/grass.png";

    auto flesh_tex = Texture();
    flesh_tex.load(flesh_tex_name);

    auto grass_tex = Texture();
    grass_tex.load(grass_tex_name);

    // set up camera
    shader.use();
    auto projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
    shader.set_mat4("projection", projection);

    // set up materials

    // flesh child material
    auto flesh_mat = Material(flesh_tex, shader, 0);
    // grass material
    auto grass_mat = Material(grass_tex, shader, 1);

    // create a renderer object and input appropriate attribute sizes and max amount of sprites on screen at once
    // 2 = pos, 2 = coords
    auto renderer = Renderer({2, 2}, 255);

    // how many columns of grass
    auto grass_cols = (int)std::ceilf(800.0f / 64.0f);
    
    // how many rows of grass
    auto grass_rows = (int)std::ceilf(600.0f / 64.0f);

    // how many total grass to draw
    auto total_grass = grass_cols * grass_rows;

    // game loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // clear screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.begin();

        // draw grass
        for (auto i = 0; i < total_grass; ++i)
        {
            Rect grass_dest
            { 
                (i % grass_cols) * 64.0f,  // finds place in column and multiplies by sprite width
                (i / grass_cols) * 64.0f,  // finds place in row and multiples by sprite height
                64.0f, 64.0f 
            };

            renderer.draw(grass_dest, src, grass_mat);
        }

        // draw flesh children
        renderer.draw(flesh_dest, src, flesh_mat);
        renderer.draw(flesh_dest2, src, flesh_mat);

        renderer.end();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window)
{
    // move character around screen
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        flesh_dest.y-= SPEED;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        flesh_dest.y+= SPEED;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        flesh_dest.x+= SPEED;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        flesh_dest.x-= SPEED;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // exit on escape key press
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}