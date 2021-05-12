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
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"

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
static float cameraPos[3] = {0.0f, 0.0f, 3.0f};
Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

static float cubeSpecular[3] = {0.5f, 0.5f, 0.5f};
static float cubeRotationX = 0.0f;
static float cubeRotationY = 0.0f;
static float cubeRotationZ = 0.0f;
static float cubeTranslation[3] = {0.0f, 0.0f, 0.0f};
static float cubeShininess = 32.0f;

static float lightColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static float lightAmbient[3] = {0.2f, 0.2f, 0.2f};
static float lightDiffuse[3] = {0.5f, 0.5f, 0.5f};
static float lightSpecular[3] = {1.0f, 1.0f, 1.0f};
static float lightPos[3] = {1.2f, 1.0f, 2.0f};

static float lightDirection[3] = {-0.2f, -1.0f, -0.3f};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};


int main(int argc, const char *argv[])
{

    Window window = Window(screenWidth, screenHeight, "Hello Color");
    window.setFramebufferSizeCallback(framebufferSizeCallback);

    /**
     * 使用GLAD来管理OpenGL的函数指针，调用任何OpenGL的函数之前都要初始化GLAD
     */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initiate glad..." << std::endl;
        return -1;
    }

    //物品的执行shader执行程序
    Program boxProgram = Program();
    Shader vertexShader = Shader("/Users/rain/Documents/projects/LearnOpenGL/08_color/shaders/shader.vs", GL_VERTEX_SHADER);
    Shader fragShader = Shader("/Users/rain/Documents/projects/LearnOpenGL/08_color/shaders/shader.fs", GL_FRAGMENT_SHADER);
    boxProgram.bindShader(vertexShader.getShader());
    boxProgram.bindShader(fragShader.getShader());
    boxProgram.link();

    Program lightProgram = Program();
    Shader lightVertexShader = Shader("/Users/rain/Documents/projects/LearnOpenGL/08_color/shaders/lightShader.vs", GL_VERTEX_SHADER);
    Shader lightFragShader = Shader("/Users/rain/Documents/projects/LearnOpenGL/08_color/shaders/lightShader.fs", GL_FRAGMENT_SHADER);
    lightProgram.bindShader(lightVertexShader.getShader());
    lightProgram.bindShader(lightFragShader.getShader());
    lightProgram.link();

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    //法向量属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //light位置属性
    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);

    //绘制imgui
    const char *glsl_version = "#version 330";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    Texture texture1 = Texture(GL_TEXTURE_2D, GL_TEXTURE0);
    texture1.setImageData("/Users/rain/Documents/projects/LearnOpenGL/08_color/container2.png", GL_RGBA, true);
    Texture texture2 = Texture(GL_TEXTURE_2D, GL_TEXTURE1);
    texture2.setImageData("/Users/rain/Documents/projects/LearnOpenGL/08_color/container2_specular.png", GL_RGBA, true);
    Texture texture3 = Texture(GL_TEXTURE_2D, GL_TEXTURE2);
    texture3.setImageData("/Users/rain/Documents/projects/LearnOpenGL/08_color/matrix.jpeg", GL_RGB, false);

    while (!window.isWindowShouldClose())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        window.processInput();
        otherProcessInput(window.getWindow());

        //隐藏光标，捕获焦点
        // glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // glfwSetCursorPosCallback(window.getWindow(), mouseCallback);
        // glfwSetScrollCallback(window.getWindow(), scrollCallback);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //创建imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Edit attribute");

        ImGui::SliderFloat3("camera pos", cameraPos, -10.0f, 10.0f);
        ImGui::SliderFloat("cube shininess", &cubeShininess, 0, 256.0f);
        ImGui::SliderFloat("cube rotation x", &cubeRotationX, 0, 360.0f);
        ImGui::SliderFloat("cube rotation y", &cubeRotationY, 0, 360.0f);
        ImGui::SliderFloat("cube rotation z", &cubeRotationZ, 0, 360.0f);
        ImGui::SliderFloat3("cube translation", cubeTranslation, -5.0f, 5.0f);
        ImGui::ColorEdit3("light color", lightColor);

        ImGui::SliderFloat3("light ambient", lightAmbient, 0, 1.0f);
        ImGui::SliderFloat3("light diffuse", lightDiffuse, 0, 1.0f);
        ImGui::SliderFloat3("light specular", lightSpecular, 0, 1.0f);
        ImGui::SliderFloat3("light pos", lightPos, -10.0f, 10.0f);

        ImGui::SliderFloat3("light direction", lightDirection, -10.0f, 0);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        boxProgram.use();
        boxProgram.setInt("material.diffuse", 0);
        boxProgram.setInt("material.specular", 1);
        boxProgram.setInt("material.emission", 2);
        
        // glm::mat4 model(1.0f);
        // model = glm::translate(model, glm::vec3(cubeTranslation[0], cubeTranslation[1], cubeTranslation[2]));
        // model = glm::rotate(model, glm::radians(cubeRotationX), glm::vec3(1.0f, 0.0f, 0.0f));
        // model = glm::rotate(model, glm::radians(cubeRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::rotate(model, glm::radians(cubeRotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

        camera.position = glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]);
        glm::mat4 view;
        view = camera.getViewMatrix();


        glm::mat4 projection(1.0f);
        projection = glm::perspective(camera.zoom, (float)screenWidth / screenHeight, 0.1f, 100.0f);

        boxProgram.setFloat("material.shininess", cubeShininess);

        boxProgram.setVec3("light.ambient", lightAmbient[0],lightAmbient[1], lightAmbient[2]);
        boxProgram.setVec3("light.diffuse", lightDiffuse[0],lightDiffuse[1], lightDiffuse[2]);
        boxProgram.setVec3("light.specular", lightSpecular[0],lightSpecular[1], lightSpecular[2]);

        boxProgram.setVec3("lightColor", lightColor[0], lightColor[1], lightColor[2]);
        // boxProgram.setMatrix("model", 1, GL_FALSE, glm::value_ptr(model));
        boxProgram.setMatrix("view", 1, GL_FALSE, &view[0][0]);
        boxProgram.setMat4("projection", 1, GL_FALSE, projection);


        boxProgram.setVec3("light.direction", lightDirection[0], lightDirection[1], lightDirection[2]);
        // boxProgram.setVec3("light.position", lightPos[0], lightPos[1], lightPos[2]);
        boxProgram.setVec3("viewPos", camera.position);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model(1.0f);
            float angle = 20.0f * i;
            model = glm::translate(model, glm::vec3(cubePositions[i]));
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::translate(model, glm::vec3(cubeTranslation[0], cubeTranslation[1], cubeTranslation[2]));
            model = glm::rotate(model, glm::radians(cubeRotationX), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(cubeRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(cubeRotationZ), glm::vec3(0.0f, 0.0f, 1.0f));
            boxProgram.setMatrix("model", 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //绘制第一个箱体
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        // lightProgram.use();
        // //绘制灯源
        // model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(lightPos[0], lightPos[1], lightPos[2]));
        // model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube
        // lightProgram.setVec3("lightColor", lightColor[0], lightColor[1], lightColor[2]);
        // lightProgram.setMatrix("model", 1, GL_FALSE, glm::value_ptr(model));
        // lightProgram.setMatrix("view", 1, GL_FALSE, &view[0][0]);
        // lightProgram.setMat4("projection", 1, GL_FALSE, projection);

        // glBindVertexArray(lightVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window.getWindow());
        glfwPollEvents(); //检查是否触发事件
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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