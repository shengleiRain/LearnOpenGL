#include "Shader.h"
#include "util.h"

Shader::Shader(const char *sourcePath, unsigned int type)
{
    this->sourcePath = sourcePath;
    this->type = type;
    compile();
}

Shader::~Shader()
{
}

void Shader::compile()
{
    const char *sourceCode = readFile(sourcePath);
    this->shader = glCreateShader(type);
    glShaderSource(this->shader, 1, &sourceCode, NULL);
    glCompileShader(this->shader);

    int success;
    char infoLog[512];
    glGetShaderiv(this->shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(this->shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
}

unsigned int Shader::getShader()
{
    return this->shader;
}