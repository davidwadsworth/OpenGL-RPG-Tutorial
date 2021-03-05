#include "component_shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Component::Shader::compile(const GLchar* vs_data, const GLchar* fs_data)
{
    // compile debug flags
    GLint success;
    GLchar info_log[512];

    // compile shaders
    GLuint vs, fs;

    // vertex shader
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_data, nullptr);
    glCompileShader(vs);

    // check for compile errors
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs, 512, nullptr, info_log);
        std::cerr << "Vertex shader compilation failure: vs = " << vs_file_name_ << std::endl;
        throw;
    }

    // fragment shader
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_data, nullptr);
    glCompileShader(fs);

    // compile errors
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fs, 512, nullptr, info_log);
        std::cerr << "Fragment shader compilation failure: fs = " << fs_file_name_ << std::endl;
    }

    // shader program init
    id_ = glCreateProgram();
    glAttachShader(id_, vs);
    glAttachShader(id_, fs);
    glLinkProgram(id_);

    // linking errors
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id_, 512, nullptr, info_log);
        std::cerr << "Program linking failure: vs = " << vs_file_name_ << "fs = " << fs_file_name_ << std::endl;
        throw;
    }

    // delete orphaned shader files
    glDeleteShader(vs);
    glDeleteShader(fs);

}

void Component::Shader::load(const GLchar* vs_file_name, const GLchar* fs_file_name)
{
    vs_file_name_ = vs_file_name;
    fs_file_name_ = fs_file_name;

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
        throw;
    }

    compile(vs_code.c_str(), fs_code.c_str());
}

void Component::Shader::use()
{
	glUseProgram(id_);
}

GLint Component::Shader::get_attrib_location(const GLchar* attrib_name)
{
    return glGetAttribLocation(id_, attrib_name);
}

void Component::Shader::set_bool(const GLchar* name, GLboolean value)
{
	glUniform1i(glGetUniformLocation(id_, name), value);
}

void Component::Shader::set_int(const GLchar* name, GLint value)
{
	glUniform1i(glGetUniformLocation(id_, name), value);
}

void Component::Shader::set_float(const GLchar* name, GLfloat value)
{
	glUniform1f(glGetUniformLocation(id_, name), value);
}

void Component::Shader::set_vec2f(const GLchar* name, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(id_, name), 1, &value[0]);
}

void Component::Shader::set_vec2f(const GLchar* name, GLfloat x, GLfloat y)
{
	glUniform2f(glGetUniformLocation(id_, name), x, y);
}

void Component::Shader::set_vec3f(const GLchar* name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(id_, name), 1, &value[0]);
}

void Component::Shader::set_vec3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z)
{
	glUniform3f(glGetUniformLocation(id_, name), x, y, z);
}

void Component::Shader::set_vec4f(const GLchar* name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(id_, name), 1, &value[0]);
}

void Component::Shader::set_vec4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	glUniform4f(glGetUniformLocation(id_, name), x, y, z, w);
}

void Component::Shader::set_mat2(const GLchar* name, const glm::mat2& matrix)
{
	glUniformMatrix2fv(glGetUniformLocation(id_, name), 1, GL_FALSE, &matrix[0][0]);
}

void Component::Shader::set_mat3(const GLchar* name, const glm::mat3& matrix)
{
	glUniformMatrix3fv(glGetUniformLocation(id_, name), 1, GL_FALSE, &matrix[0][0]);
}

void Component::Shader::set_mat4(const GLchar* name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, GL_FALSE, &matrix[0][0]);
}
