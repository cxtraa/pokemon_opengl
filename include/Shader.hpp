#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader {
protected:
    GLuint shaderProgram;
public:

    Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
    Shader();
    void use();
    void setFloat(const std::string& name, float value) const;
    void setInt(const std::string& name, int value) const;
    void setBool(const std::string& name, bool value) const;
};