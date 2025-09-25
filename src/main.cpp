
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include "constants.hpp"
#include "Drawer.hpp"
#include "SpriteAnimation.hpp"
#include "Screen.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "DialogueManager.hpp"
#include "Direction.hpp"
#include "stb_image.h"

void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

int main(int argc, char* argv[]) {
    glfwSetErrorCallback(error_callback);
    Screen& screen = Screen::getInstance();
    GLFWwindow* window = screen.getWindow();
    Game& game = Game::getInstance();
    glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        screen.clear();
        game.draw();
        screen.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}