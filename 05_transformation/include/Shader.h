#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader;

class Shader
{
private:
    //着色器源代码文件路径
    const char *sourcePath;
    //着色器的类型
    unsigned int type;
    //着色器地址
    unsigned int shader;

    void compile();

public:
    Shader(const char *sourcePath, unsigned int type);
    ~Shader();
    unsigned int getShader();
};

#endif
