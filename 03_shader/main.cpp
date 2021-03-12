#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Program.h"
#include "Shader.h"

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void loopFun(Window *window);

int main(int argc, const char *argv[])
{
    Window window = Window(400, 300, "Hello Shader");
    window.setFramebufferSizeCallback(framebufferSizeCallback);

    /**
     * 使用GLAD来管理OpenGL的函数指针，调用任何OpenGL的函数之前都要初始化GLAD
     */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initiate glad..." << std::endl;
        return -1;
    }

    //执行shader的程序
    Program program = Program();
    Shader vertexShader = Shader("/Users/rain/Documents/projects/LearnOpenGL/03_shader/shaders/shader.vs", GL_VERTEX_SHADER);
    Shader fragShader = Shader("/Users/rain/Documents/projects/LearnOpenGL/03_shader/shaders/shader.fs", GL_FRAGMENT_SHADER);
    program.bindShader(vertexShader.getShader());
    program.bindShader(fragShader.getShader());
    program.link();

    float vertices[] = {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    //颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!window.isWindowShouldClose())
    {
        window.processInput();

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();
        float offsetX = 0.5f;
        program.setFloat("offsetX", offsetX);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window.getWindow());
        glfwPollEvents(); //检查是否触发事件
    }

    return 0;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    std::cout << "size changed...width=" << width << " height=" << height << std::endl;
    glViewport(0, 0, width, height);
}