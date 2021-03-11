#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main(int argc, const char *argv[])
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
    GLFWwindow *window = glfwCreateWindow(800, 600, "HELLO GL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed create GLFW window..." << std::endl;
        glfwTerminate(); //终止
        return -1;
    }

    glfwMakeContextCurrent(window);

    /**
     * 使用GLAD来管理OpenGL的函数指针，调用任何OpenGL的函数之前都要初始化GLAD
     */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initiate glad..." << std::endl;
        return -1;
    }

    //设置callback，每当窗口调整大小的时候，就会调用framebufferSizeCallback函数，以此来重新渲染视图
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    //渲染循环
    while (!glfwWindowShouldClose(window))
    {
        //处理键盘操作， 输入
        processInput(window);

        //渲染指令
        glClearColor(0.2f, 0.3f, 0.1f, 0.4f); 
        /**
         * 当调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色。
         * 当前设置的只是颜色缓冲通道，GL_COLOR_BUFFER_BIT， 此外还有其他缓冲通道，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT
         */ 
        glClear(GL_COLOR_BUFFER_BIT); 
        /**
         * 交换颜色缓冲，使用双缓冲，减少视觉上的不真实感和闪烁---
         * 前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。
         * 当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓冲，这样图像就立即呈显出来，之前提到的不真实感就消除了。
         */ 
        glfwSwapBuffers(window); 
        glfwPollEvents(); //检查是否触发事件
    }

    //退出时释放所有的资源
    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    std::cout << "size changed...width=" << width << " height=" << height << std::endl;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        std::cout << "click esc close window..." << std::endl;
        glfwSetWindowShouldClose(window, true);
    }
}
