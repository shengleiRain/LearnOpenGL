#include "util.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string readFile(const char *path)
{
    std::string result;
    std::ifstream iStreamFile;
    //保证ifstream可以抛出异常
    iStreamFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        iStreamFile.open(path);
        std::stringstream stream;
        stream << iStreamFile.rdbuf();
        iStreamFile.close();
        result = stream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.what() << std::endl;
    }
    return result;
}