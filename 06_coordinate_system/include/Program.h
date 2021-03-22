#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <list>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Program;

class Program
{
private:
    unsigned int id;
    std::list<unsigned int> shaders;

public:
    Program();
    ~Program();

    //使用/激活程序
    void use();
    // uniform工具函数
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setFloats(const std::string &name, float value1, float value2, float value3, float value4) const;
    void setMatrix(const std::string &name, unsigned int count, bool transpose, float *value) const;
    void setMat4(const std::string &name, unsigned int count, bool transpose, glm::mat4 &matrix) const;

    Program *bindShader(unsigned int shader);
    void link();

    unsigned int getId();
};

#endif