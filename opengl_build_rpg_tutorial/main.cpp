#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "shader.h"
#include "texture.h"

/*
Source code for episode 3 of Build Your Own RPG series

@author David Wadsworth
*/

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

    // bind texture for drawing
    texture.bind();

    // set up vertex data
    GLfloat vertices[] = {
    // first triangle
        // pos         // coords
        -0.5f, -0.5f,   0.0f,  0.0f, // bot left 
         0.5f,  0.5f,   1.0f,  1.0f, // top right 
        -0.5f,  0.5f,   0.0f,  1.0f, // top left 
    // second triangle
        // pos         // coords
        -0.5f, -0.5f,   0.0f,  0.0f, // bot left 
         0.5f, -0.5f,   1.0f,  0.0f, // bot right 
         0.5f,  0.5f,   1.0f,  1.0f  // top right 
    };

    GLuint vbo, vao;

    // create storage in gpu
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    // set up vbo data inside gpu
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set up attributes to decipher vbo in gpu
    
    // position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2* sizeof(float)));
    glEnableVertexAttribArray(1);

    // game loop
    while (!glfwWindowShouldClose(window))
    {
        // clear screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw image
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // exit on escape key press
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    glfwTerminate();

    return 0;
}