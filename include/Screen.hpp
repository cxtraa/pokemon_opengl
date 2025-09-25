#pragma once

#include "Shader.hpp"
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Screen {
protected:
    Screen();
    Screen(const Screen&) = delete;
    Screen& operator=(const Screen&) = delete;
public:
    GLuint textureID, VAO, VBO, EBO;
    Shader shaderProgram;
    GLFWwindow* window;
    std::vector<unsigned char> buffer;

    static Screen& getInstance();
    GLFWwindow* getWindow();
    ~Screen();

    void drawPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void render();
    void clear();
};