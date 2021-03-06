#include "component_shader.h"
#include <fstream>
#include <sstream>
#include "logger.h"

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
        Logger::error("Vertex shader compilation failure: vs = " + vs_file_name_ + "\n" + std::string(info_log), Logger::MEDIUM);
        glDeleteShader(vs);
        return;
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
        Logger::error("Fragment shader compilation failure: fs = " + fs_file_name_ + "\n" + std::string(info_log), Logger::MEDIUM);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return;
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
        Logger::error("Program linking failure: vs = " + vs_file_name_ + "fs = " + fs_file_name_ + "\n" + std::string(info_log), Logger::MEDIUM);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return;
    }

    // delete orphaned shader files
    glDeleteShader(vs);
    glDeleteShader(fs);

}

void Component::Shader::load(const GLchar* vs_file_name, const GLchar* fs_file_name)
{
    vs_file_name_ = vs_file_name;
    fs_file_name_ = fs_file_name;

    std::string vs_code, fs_code;

    try
    {
        // open vs shader files
        std::ifstream vs_file(vs_file_name_);
        std::stringstream vs_sstream;

        vs_sstream << vs_file.rdbuf();

        vs_file.close();

        vs_code = vs_sstream.str();
    }
    catch (std::exception e)
    {
        Logger::error("Failed to read vs shader file, " + std::string(vs_file_name_), Logger::MEDIUM);
        return;
    }

    try
    {
        // open fs shader files
        std::ifstream fs_file(fs_file_name_);
        std::stringstream fs_sstream;

        fs_sstream << fs_file.rdbuf();

        fs_file.close();

        fs_code = fs_sstream.str();

    }
    catch (std::exception e)
    {
        Logger::error("Failed to read fs shader file, " + std::string(fs_file_name_), Logger::MEDIUM);
        return;
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
