#version 330 core

layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec2 aTexCoords;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //计算法线矩阵，为了让物体执行缩放的时候，法线依然有效
    normal=mat3(transpose(inverse(model)))*aNormal;
    fragPos=vec3(model*vec4(aPos,1.0));
    gl_Position=projection*view*vec4(fragPos,1.0);
    texCoords = aTexCoords;
}
