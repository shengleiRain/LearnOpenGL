#include "Texture.h"
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <iostream>

Texture::Texture(int size, int activeId)
{
    this->size = size;
    this->activeId = activeId;
    //生成纹理
    glGenTextures(1, &texture);
    glActiveTexture(activeId);
    glBindTexture(size, texture);

    //为当前的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

Texture::~Texture()
{
}

void Texture::setImageData(const char *imagePath, int format, bool flipVertically)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(size, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(size);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

int Texture::getSize()
{
    return this->size;
}

unsigned int Texture::getTexture()
{
    return this->texture;
}

void Texture::bind()
{
    glActiveTexture(activeId);
    glBindTexture(size, texture);
}