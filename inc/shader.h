#pragma once

#include <iostream>
#include <math.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <streambuf>

class Shader {

public:
    GLuint ID;

    Shader(const std::string &vertexShaderScript, const std::string &fragmentShaderScript);
    void use();

    void setUniform(const std::string &name, const float &value) const;
    void setUniform(const std::string &name, const int &value) const;
    void setUniform(const std::string &name, const bool &value) const;
    void setUniform(const std::string &name, const glm::mat4 &value) const;
    void setUniform(const std::string &name, const glm::vec3 &value) const;

};
