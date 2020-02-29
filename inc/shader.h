#pragma once

#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <streambuf>

class Shader {

public:
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    void use();

    void setUniform(const std::string &name, float value) const;
    void setUniform(const std::string &name, int value) const;
    void setUniform(const std::string &name, bool value) const;
    void setUniform(const std::string &name, glm::mat4 value) const;
    void setUniform(const std::string &name, glm::vec3 value) const;

};
