## Hello Triangle
[参考链接](https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/)
在这一个demo中，我们将学习如何使用OpenGL绘制一个三角形。在OpenGL中，任何事物都在3D空间中，而我们显示的屏幕是2D像素数组，所以要想使图形在屏幕上显示，就需要将其转化。这种3D坐标向2D坐标转化的过程是由**图形渲染管线**完成的，转变过程的流程图如下图所示。

![img](https://learnopengl-cn.github.io/img/01/04/pipeline.png)
其中蓝色部分是可自定义的着色器，灰色部分是固定的着色器。

### 顶点数据
- ***顶点数据***是一系列顶点的集合，
- ***顶点***是一个3D坐标数据的集合，
- ***顶点的数据***是用***顶点属性***表示的，可以表示任何数据，不过简单起见，包括3D位置和颜色。

要在屏幕上显示图形，首先需要准备输入的顶点数据。并且需要将其缓存到显存中。在GLAD库中，使用**顶点缓冲对象(Vertex Buffer Object, VBO)**来管理这个内存地址，并且在显存中为该对象申请内存。OpenGL允许绑定多个缓冲，只要是不同类型的。在本文中，将其绑定到GL_ARRAY_BUFFER中，并且将顶点数据复制到缓冲的内存中。相关代码如下：
```c++
unsigned int VBO;
glGenBuffers(1, &VBO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

### 顶点着色器 (Vertex Shader)
顶点着色器是可编程着色器之一。主要的作用在于将输入的顶点数据中的每一个顶点3D坐标转换成另一种3D坐标，并且可以对顶点属性做一些基本的处理。

使用着色器语言（GLSL）编写顶点着色器，并且编译这个着色器。例如一个很基本的顶点着色器：
```glsl
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```
然后在c++代码中进行硬编码，并且动态编译源代码。
```c++
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
//创建着色器
unsigned int vertexShader;
vetexShader = glCreateShader(GL_VERTEX_SHADER);
//附着上源代码，并且编译
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShader);
```

### 图元装配 （Shape Assembly）
将顶点着色器输出的所有顶点作为输入，并将所有的点装配成指定图元的形状。

### 几何着色器 （Geometry Shader）
几何着色器把图元形式的一系列顶点的集合作为输入，它可以通过产生新顶点构造出新的（或是其它的）图元来生成其他形状。

### 光栅化 （Resterizatoin）
将图元转化成像素

### 片段着色器 （Fragment Shader）
主要目的是计算一个像素的最终颜色，这也是所有OpenGL高级效果产生的地方。通常，片段着色器包含3D场景的数据（比如光照、阴影、光的颜色等等），这些数据可以被用来计算最终像素的颜色。

### Alpha测试和混合
