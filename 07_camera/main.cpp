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

#include "Camera.h"

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void otherProcessInput(GLFWwindow *window);
void loopFun(Window *window);
void mouseCallback(GLFWwindow *window, double xPos, double yPos);
void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);

int screenWidth = 800;
int screenHeight = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastXPos = 400;
float lastYPos = 300;

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main(int argc, const char *argv[])
{
    Window window = Window(screenWidth, screenHeight, "Hello Camera");
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
    Shader vertexShader = Shader("/Users/rain/Documents/projects/LearnOpenGL/07_camera/shaders/shader.vs", GL_VERTEX_SHADER);
    Shader fragShader = Shader("/Users/rain/Documents/projects/LearnOpenGL/07_camera/shaders/shader.fs", GL_FRAGMENT_SHADER);
    program.bindShader(vertexShader.getShader());
    program.bindShader(fragShader.getShader());
    program.link();

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Texture texture1 = Texture(GL_TEXTURE_2D, GL_TEXTURE0);
    texture1.setImageData("/Users/rain/Documents/projects/LearnOpenGL/07_camera/container.jpeg", GL_RGB, false);
    Texture texture2 = Texture(GL_TEXTURE_2D, GL_TEXTURE1);
    texture2.setImageData("/Users/rain/Documents/projects/LearnOpenGL/07_camera/awesomeface.png", GL_RGBA, true);

    program.use();
    program.setInt("texture1", 0);
    program.setInt("texture2", 1);

    glEnable(GL_DEPTH_TEST);

    while (!window.isWindowShouldClose())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        window.processInput();
        otherProcessInput(window.getWindow());

        //隐藏光标，捕获焦点
        glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window.getWindow(), mouseCallback);
        glfwSetScrollCallback(window.getWindow(), scrollCallback);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.use();
        texture1.bind();
        texture2.bind();

        glm::mat4 model(1.0f);
        // model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // glm::mat4 view(1.0f);
        // view = lm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 view;
        view = camera.getViewMatrix();

        glm::mat4 projection(1.0f);
        projection = glm::perspective(camera.zoom, (float)screenWidth / screenHeight, 0.1f, 100.0f);

        program.setMatrix("model", 1, GL_FALSE, glm::value_ptr(model));
        program.setMatrix("view", 1, GL_FALSE, &view[0][0]);
        program.setMat4("projection", 1, GL_FALSE, projection);

        //绘制第一个箱体
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
}

void otherProcessInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        camera.processKeyboard(FORWORD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        camera.processKeyboard(BACKWORD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        camera.processKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        camera.processKeyboard(RIGHT, deltaTime);
    }
}

void mouseCallback(GLFWwindow *window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastXPos = xPos;
        lastYPos = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastXPos;
    float yOffset = lastYPos - yPos;
    lastXPos = xPos;
    lastYPos = yPos;
    camera.processMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    camera.processMouseScroll(yOffset);
}