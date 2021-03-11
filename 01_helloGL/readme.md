## 环境配置
首先介绍一下我的环境配置。在学习本系列项目，我是采用vscode作为编辑器，cmake来编译构建项目。使用GLFW库来显示窗口，GLAD提供OpenGL的api实现。

### GLFW的下载和配置
我是在mac上配置的，配置非常简单，只需要先安装好Homebrew，然后简单一句

```
brew install glfw
```
即可。
当然也可以自己去[官网下载](https://www.glfw.org/download.html),然后自行编译，使用cmake。推荐使用brew进行配置，简单～

### GLAD
GLFW提供了创建窗口的便利，但是OpenGL只是提供了一个规范，具体的实现因显卡而异。由于OpenGL驱动版本众多，它大多数函数的位置都无法在编译时确定下来，需要在运行时查询。所以任务就落在了开发者身上，开发者需要在运行时获取函数地址并将其保存在一个函数指针中供以后使用。

GLAD就是为了便利获得该函数地址指针而提供的。可以在[在线服务](https://glad.dav1d.de/)中根据使用的平台来生成特定的代码。

根据我使用的平台，配置如下：

- Language: c/c++
- Specification: OpenGL
- gl: 3.3
- Profile: Core

获得后，将include中的文件复制到自己项目中的include文件夹里面，src也需要复制过来。

### VSCode + Cmake
在vs code中如何创建项目的就不说了，主要讲一下cmake的配置，在vscode中，使用【shift+cmd+p】调出命令面板，输入【Cmake+Quick Start】就能生成Cmake配置。将CMakeLists.txt中替代成如下：
```cmake
# 添加头文件
set(GLAD_H ${PROJECT_SOURCE_DIR}/include)
set(GLFW_H /usr/local/include)
include_directories(${GLAD_H} ${GLFW_H})

# 添加目标链接
set(GLFW_LINK /usr/local/lib/libglfw.3.dylib)
link_libraries(${GLFW_LINK})

# 执行编译命令
set(SOURCES ${PROJECT_SOURCE_DIR}/src/glad.c main.cpp)
add_executable(HelloGL ${SOURCES})

# 链接系统的 OpenGL 框架
if (APPLE)
    target_link_libraries(HelloGL "-framework OpenGL")
endif()

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

```
可根据自己的需求，修改以上内容。


## 你好，OpenGL
这是我开始OpenGL学习的开始，希望能在这个项目中，能够学到学到一些东西。

本文是参考[你好，窗口](https://learnopengl-cn.github.io/01%20Getting%20started/03%20Hello%20Window/)进行coding。

### 主要步骤
- 配置glfw的一些信息，初始化，最大最小版本，向前兼容等
- 创建一个窗口
- 获取OpenGL的函数指针
- 渲染
- 退出，释放资源

### vs code 运行
调出命令面板，输入【Cmake:Build】进行编译，输入【Cmake:Run Without Debugging】编译运行。
