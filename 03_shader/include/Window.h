#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

//前置声明
class Window;
class Window
{
private:
    GLFWwindow *window;
    unsigned int width;
    unsigned int height;
    char *title;
    void init();

public:

    const unsigned int WIDTH = 800;
    const unsigned int HEIGHT = 600;
    const char *TITLE = "HELLO GL";

    Window();
    Window(unsigned int width, unsigned int height, const char *title);
    ~Window();


    Window *setFramebufferSizeCallback(GLFWframebuffersizefun callback);
    void processInput();
    GLFWwindow *getWindow();
    bool isWindowShouldClose();
};

#endif
