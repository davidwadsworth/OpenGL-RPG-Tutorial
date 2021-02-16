#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>

/*
Source code for episode 2 of Build Your Own RPG series

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

    auto vs_file_name = "resources/shaders/triangle.vs";
    auto fs_file_name = "resources/shaders/triangle.fs";

    // load shaders from file
    std::string vs_code;
    std::string fs_code;

    try
    {
        // open files
        std::ifstream vs_file(vs_file_name);
        std::ifstream fs_file(fs_file_name);
        std::stringstream vs_stream, fs_stream;

        // read into temp string streams
        vs_stream << vs_file.rdbuf();
        fs_stream << fs_file.rdbuf();

        // close file streams
        vs_file.close();
        fs_file.close();

        // convert streams into strings
        vs_code = vs_stream.str();
        fs_code = fs_stream.str();
    }
    catch (std::exception e)
    {
        std::cerr << "Failed to read shader files!" << std::endl;
        return -1;
    }

    // compile debug flags
    GLint success;
    GLchar info_log[512];

    // compile shaders
    GLuint vs, fs, program;

    auto vs_code_c_str = vs_code.c_str();
    auto fs_code_c_str = fs_code.c_str();

    // vertex shader
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_code_c_str, nullptr);
    glCompileShader(vs);

    // check for compile errors
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs, 512, nullptr, info_log);
        std::cerr << "Vertex shader compilation failure: vs = " << vs_file_name << std::endl;
        return -1;
    }

    // fragment shader
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_code_c_str, nullptr);
    glCompileShader(fs);

    // compile errors
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fs, 512, nullptr, info_log);
        std::cerr << "Fragment shader compilation failure: fs = " << fs_file_name << std::endl;
        return -1;
    }

    // shader program init
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    // linking errors
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, nullptr, info_log);
        std::cerr << "Program linking failure: vs = " << vs_file_name << "fs = " << fs_file_name << std::endl;
        return -1;
    }

    // delete orphaned shader files
    glDeleteShader(vs);
    glDeleteShader(fs);

    // use program for drawing
    glUseProgram(program);

    // set up vertex data for vs
    GLfloat vertices[] = {
        -1.0f, -1.0f, // left
         1.0f, -1.0f, // right
         0.0f,  1.0f  // top
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // game loop
    while (!glfwWindowShouldClose(window))
    {
        // clear screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // exit on escape key press
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    glfwTerminate();

    return 0;
}