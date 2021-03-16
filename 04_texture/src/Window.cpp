#include "Window.h"

void Window::init()
{
    /**
     * 配置glfw的一些信息
     */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__ //MAC OS X 系统需要设置向前兼容
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    /**
     * 创建一个window
     */
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed create GLFW window..." << std::endl;
        glfwTerminate(); //终止
        return;
    }

    glfwMakeContextCurrent(window);
}

Window *Window::setFramebufferSizeCallback(GLFWframebuffersizefun callback)
{
    glfwSetFramebufferSizeCallback(window, callback);
    return this;
}

void Window::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        std::cout << "click esc close window..." << std::endl;
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow *Window::getWindow()
{
    return window;
}

bool Window::isWindowShouldClose()
{
    return glfwWindowShouldClose(window);
}

Window::Window()
{
    width = WIDTH;
    height = HEIGHT;
    title = const_cast<char *>(TITLE);
    init();
}

Window::Window(unsigned int width, unsigned int height, const char *title)
{
    this->width = width;
    this->height = height;
    this->title = const_cast<char *>(title);
    init();
}

Window::~Window()
{
    std::cout << "terminate window..." << std::endl;
    glfwTerminate();
}
