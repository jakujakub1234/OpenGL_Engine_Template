#include "../../include/engine_core/ShaderManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void ShaderManager::BuildShaders(char* vs_filename, char* fs_filename)
{
    std::ifstream vs_file;
    vs_file.open(vs_filename);
    std::stringstream vs_stream;
    vs_stream << vs_file.rdbuf();		
    vs_file.close();
    std::string vs_code = vs_stream.str();
    const char* vs_shader = vs_code.c_str();

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vs_shader, NULL);
    glCompileShader(vertex_shader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    std::ifstream fs_file;
    fs_file.open(fs_filename);
    std::stringstream fs_stream;
    fs_stream << fs_file.rdbuf();		
    fs_file.close();
    std::string fs_code = fs_stream.str();
    const char* fs_shader = fs_code.c_str();

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fs_shader, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    this->shader_program = glCreateProgram();
    glAttachShader(this->shader_program, vertex_shader);
    glAttachShader(this->shader_program, fragment_shader);
    glLinkProgram(this->shader_program);

    glGetProgramiv(this->shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->shader_program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void ShaderManager::Use()
{
    glUseProgram(this->shader_program);
}

void ShaderManager::SetFloat(GLchar* name, float value)
{
    glUniform1f(glGetUniformLocation(this->shader_program, name), value);
}

void ShaderManager::SetInt(GLchar* name, int value)
{
    glUniform1i(glGetUniformLocation(this->shader_program, name), value);
}

void ShaderManager::SetMat4(GLchar* name, const glm::mat4& mat)
{
    glUniformMatrix4fv(glGetUniformLocation(this->shader_program, name), 1, GL_FALSE, &mat[0][0]);
}

void ShaderManager::DeleteShader()
{
    glDeleteProgram(this->shader_program);
}