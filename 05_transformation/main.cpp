#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Program.h"
#include "Shader.h"
#include <stb/stb_image.h>
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void otherProcessInput(GLFWwindow *window, Program *program, float rate);
void loopFun(Window *window);

int main(int argc, const char *argv[])
{
    Window window = Window(800, 600, "Hello Transformation");
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
    Shader vertexShader = Shader("/Users/rain/Documents/projects/LearnOpenGL/05_transformation/shaders/shader.vs", GL_VERTEX_SHADER);
    Shader fragShader = Shader("/Users/rain/Documents/projects/LearnOpenGL/05_transformation/shaders/shader.fs", GL_FRAGMENT_SHADER);
    program.bindShader(vertexShader.getShader());
    program.bindShader(fragShader.getShader());
    program.link();

    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    //颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Texture texture1 = Texture(GL_TEXTURE_2D, GL_TEXTURE0);
    texture1.setImageData("/Users/rain/Documents/projects/LearnOpenGL/04_texture/container.jpeg", GL_RGB, false);
    Texture texture2 = Texture(GL_TEXTURE_2D, GL_TEXTURE1);
    texture2.setImageData("/Users/rain/Documents/projects/LearnOpenGL/04_texture/awesomeface.png", GL_RGBA, true);

    program.use();
    program.setInt("texture1", 0);
    program.setInt("texture2", 1);

    while (!window.isWindowShouldClose())
    {
        window.processInput();

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();
        texture1.bind();
        texture2.bind();

        glm::mat4 trans(1.0f);
        trans = glm::translate(trans,glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        program.setMatrix("transform", 1, GL_FALSE, glm::value_ptr(trans));

        //绘制第一个箱体
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scaleAmount = sin(glfwGetTime());
        trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        program.setMatrix("transform", 1, GL_FALSE, &trans[0][0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window.getWindow());
        glfwPollEvents(); //检查是否触发事件
    }

    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    return 0;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    std::cout << "size changed...width=" << width << " height=" << height << std::endl;
    glViewport(0, 0, width, height);
}