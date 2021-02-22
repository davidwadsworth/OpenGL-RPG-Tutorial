#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "shader.h"
#include "texture.h"
#include <glm/ext/matrix_clip_space.hpp>
#include "renderer.h"

/*
Source code for episode 4 of Build Your Own RPG series

@author David Wadsworth
*/

void processInput(GLFWwindow* window);
Rect dest = { 400 - 32, 300 - 32, 64, 64 };
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

    auto window = glfwCreateWindow(800, 640, "IntelligenceQuest", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "failed to initialize glad!" << std::endl;
        return -1;
    }

    // load in used shaders
    auto vs_file_name = "resources/shaders/sprite.vs";
    auto fs_file_name = "resources/shaders/sprite.fs";

    auto shader = Shader();
    shader.load(vs_file_name, fs_file_name);

    // load in used textures
    auto tex_file_name = "resources/images/flesh_child_full_1.png";

    auto texture = Texture();
    texture.load(tex_file_name);

    // use program for drawing
    shader.use();
    
    // set up the tex unit to the correct value
    shader.set_int("image", 0);

    // set up 2d camera
    auto projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
    shader.set_mat4("projection", projection);

    // bind texture for drawing
    texture.bind();

    // create a renderer object and input appropriate attribute sizes
    // 2 = pos, 2 = coords
    auto renderer = Renderer({2, 2});

    // game loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // clear screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw rects
        renderer.draw(dest, src);

        // flush draw calls
        renderer.flush();

        glfwSwapBuffers(window);
        glfwPollEvents();

        // exit on escape key press
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        dest.y-= SPEED;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        dest.y+= SPEED;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        dest.x+= SPEED;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        dest.x-= SPEED;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}