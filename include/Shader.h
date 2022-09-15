//
// Created by inver on 2022/9/12.
//

#ifndef __SHADER_H__
#define __SHADER_H__

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int id;

    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    void use() const;
    // uniform工具函数
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};


#endif
