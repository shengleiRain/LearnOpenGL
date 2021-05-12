cmake_minimum_required(VERSION 3.0.0)
project(GL_Color VERSION 0.1.0)

# 使用 C++ 11 标准
set(CMAKE_CXX_STANDARD 11)

# 添加头文件
set(GLAD_H ${PROJECT_SOURCE_DIR}/include)
set(GLFW_H /usr/local/include)
set(IMGUI_H ${PROJECT_SOURCE_DIR}/include/imgui)
include_directories(${GLAD_H} ${GLFW_H} ${IMGUI_H})

# 添加目标链接
set(GLFW_LINK /usr/local/lib/libglfw.3.dylib)
link_libraries(${GLFW_LINK})

# 执行编译命令
set(SOURCES main.cpp)
aux_source_directory(${PROJECT_SOURCE_DIR}/src DIR_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/include/imgui IMGUI_SRC)
add_executable(GL_Color ${SOURCES} ${DIR_SRC} ${IMGUI_SRC})

# 链接系统的 OpenGL 框架
if (APPLE)
    target_link_libraries(GL_Color "-framework OpenGL")
endif()

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
