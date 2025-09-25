
#include "Shader.hpp"
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath) {
    std::fstream vertexShaderFile(vertexShaderPath);
    std::fstream fragmentShaderFile(fragmentShaderPath);

    std::stringstream vertexShaderSS;
    std::stringstream fragmentShaderSS;

    vertexShaderSS << vertexShaderFile.rdbuf();
    fragmentShaderSS << fragmentShaderFile.rdbuf();

    std::string vertexShaderStr = vertexShaderSS.str();
    std::string fragmentShaderStr = fragmentShaderSS.str();

    const char* vertexShaderSource = vertexShaderStr.c_str();
    const char* fragmentShaderSource = fragmentShaderStr.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::Shader() : shaderProgram(0) {}

void Shader::use() {
    glUseProgram(shaderProgram);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}