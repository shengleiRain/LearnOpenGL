#include "Program.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Program::Program()
{
    id = glCreateProgram();
}

Program::~Program()
{
    glDeleteProgram(id);
}

void Program::use()
{
    glUseProgram(id);
}

void Program::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Program::setFloat(const std::string &name, float value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Program::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Program::setFloats(const std::string &name, float value1, float value2, float value3, float value4) const
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), value1, value2, value3, value4);
}

void Program::setMatrix(const std::string &name, unsigned int count, bool transpose, float *value)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), count, transpose, value);
}

void Program::link()
{
    glLinkProgram(id);
    int success;
    char infoLog[512];
    // 打印连接错误（如果有的话）
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    for (std::list<unsigned int>::iterator it = shaders.begin(); it != shaders.end(); ++it)
    {
        glDeleteShader(*it);
    }
    shaders.clear();
}

Program *Program::bindShader(unsigned int shader)
{
    shaders.push_back(shader);
    glAttachShader(id, shader);
    return this;
}

unsigned int Program::getId()
{
    return id;
}