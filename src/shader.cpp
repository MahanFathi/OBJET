#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

    // vertex shader
    std::ifstream tv(vertexPath);
    std::string vertexShaderScript((std::istreambuf_iterator<char>(tv)), std::istreambuf_iterator<char>());
    const char * vertexShaderSource = vertexShaderScript.c_str();
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // fragment shader
    std::ifstream tf(fragmentPath);
    std::string fragmentShaderScript((std::istreambuf_iterator<char>(tf)), std::istreambuf_iterator<char>());
    const char * fragmentShaderSource = fragmentShaderScript.c_str();
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // link shaders
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    ID = shaderProgram;
}

void Shader::use() {
    glUseProgram(ID);
}

// util fuctions
void Shader::setUniform(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setUniform(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setUniform(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setUniform(const std::string &name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setUniform(const std::string &name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}
