
#include "Screen.hpp"
#include "constants.hpp"
#include "stb_image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

Screen::Screen() : 
    buffer(CScreen::pixelsWidth * CScreen::pixelsHeight * 4, 0)
{
    if (!glfwInit()) {
        throw std::runtime_error("Screen: failed to initialize GLFW.");
    }
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window = glfwCreateWindow(CScreen::screenWidth, CScreen::screenHeight, appName.c_str(), NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Screen: window was not successfully created.");
    }
    glfwMakeContextCurrent(window);
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        glfwTerminate();
        throw std::runtime_error("Screen: GLEW was not initialised correctly.");
    }

    shaderProgram = Shader(CShader::vertexPath, CShader::fragmentPath);

    float vertices[] = {
        -1.0f, 1.0f, 0.0f, 1.0f,    // Top left
        -1.0f, -1.0f, 0.0f, 0.0f,   // Bottom left
        1.0f, -1.0f, 1.0f, 0.0f,    // Bottom right
        1.0f, 1.0f, 1.0f, 1.0f      // Top right
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 3, 2
    };

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CScreen::pixelsWidth, CScreen::pixelsHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, CScreen::pixelsWidth, CScreen::pixelsHeight, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
    glActiveTexture(GL_TEXTURE0);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    shaderProgram.setInt("pixelTexture", 0);
}

Screen& Screen::getInstance() {
    static Screen instance;
    return instance;
}

GLFWwindow* Screen::getWindow() {
    return window;
}

Screen::~Screen() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &textureID);
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Screen::drawPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    if (x < 0 || x >= CScreen::pixelsWidth || y < 0 || y >= CScreen::pixelsHeight) return;

    if (a == 0) return;

    y = CScreen::pixelsHeight - 1 - y;
    
    buffer[4 * (CScreen::pixelsWidth*y + x) + 0] = r;
    buffer[4 * (CScreen::pixelsWidth*y + x) + 1] = g;
    buffer[4 * (CScreen::pixelsWidth*y + x) + 2] = b;
    buffer[4 * (CScreen::pixelsWidth*y + x) + 3] = a;
}

void Screen::render() {
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, CScreen::pixelsWidth, CScreen::pixelsHeight, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
    shaderProgram.use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Screen::clear() {
    for (int i = 0; i < CScreen::pixelsHeight; i++) {
        for (int j = 0; j < CScreen::pixelsWidth; j++) {
            drawPixel(j, i, 0, 0, 0, 255);
        }
    }
};