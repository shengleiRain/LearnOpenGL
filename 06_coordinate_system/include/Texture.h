#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{
private:
    int width;
    int height;
    int nrChannels;
    unsigned char *data; //图片数据
    unsigned int texture; //纹理指针地址
    int size; //纹理对应的部分
    int activeId; //激活纹理的编号

public:
    Texture(int size, int activeId);
    ~Texture();

    void setImageData(const char* imagePath, int format, bool flipVertically);
    int getSize();
    unsigned int getTexture();
    void bind();
};

#endif